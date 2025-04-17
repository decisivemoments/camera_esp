#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"

#include "time.h"
#include "sys/time.h"
#include "spp.h"

#define SPP_TAG "SPP_task"
static const esp_spp_mode_t esp_spp_mode = ESP_SPP_MODE_CB;
static const bool esp_spp_enable_l2cap_ertm = true;

esp_bd_addr_t peer_bd_addr = {0};
static uint8_t peer_bdname_len;
static char peer_bdname[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
static const char remote_device_name[] = "STARSSSLEGION";

typedef struct {
    uint8_t *data;
    size_t len;
    spp_data_type_t type;
    uint32_t timestamp;  // 添加时间戳用于超时处理
} spp_data_packet_t;

// 配置参数
#define SPP_QUEUE_SIZE 8          // 队列大小：能存储2秒的低清图像(4fps * 2s = 8)
#define MAX_HD_PACKET_SIZE 65536  // 64KB, 足够存储高清JPEG
#define MAX_LD_PACKET_SIZE 16384  // 16KB, 足够存储低清灰度图
#define SPP_PACKET_TIMEOUT_MS 3000 // 3秒超时
#define SPP_MTU_SIZE ESP_SPP_MAX_MTU          // BLE MTU限制

//spp发送数据相关
static QueueHandle_t g_spp_queue = NULL;
static SemaphoreHandle_t g_spp_mutex = NULL;
static bool g_spp_congested = false;
static uint32_t g_spp_handle = 0;
static bool g_spp_connected = false;
static uint32_t g_packets_sent = 0;
static uint32_t g_packets_dropped = 0;
static uint32_t g_bytes_sent = 0;

//spp connect 状态相关管理
#define TARGET_DEVICE_NAME "STARSSSLEGION"
#define MAX_RETRY_COUNT 3
#define DISCOVERY_TIMEOUT_MS 10000

typedef struct {
    esp_bd_addr_t target_addr;
    bool device_found;
    bool discovery_completed;
    SemaphoreHandle_t discovery_sem;
    uint8_t retry_count;
    uint8_t remote_scn;
} spp_connection_state_t;

static spp_connection_state_t g_conn_state = {
    .device_found = false,
    .discovery_completed = false,
    .discovery_sem = NULL,
    .retry_count = 0,
    .remote_scn = 4
};

static char *bda2str(uint8_t * bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18) {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}

static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    char bda_str[18] = {0};

    switch (event) {
    case ESP_SPP_INIT_EVT:
        if (param->init.status == ESP_SPP_SUCCESS) {
            ESP_LOGI(SPP_TAG, "ESP_SPP_INIT_EVT");
        } else {
            ESP_LOGE(SPP_TAG, "ESP_SPP_INIT_EVT status:%d", param->init.status);
        }
        break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
        if (param->disc_comp.status == ESP_SPP_SUCCESS) {
            ESP_LOGI(SPP_TAG, "ESP_SPP_DISCOVERY_COMP_EVT scn_num:%d", param->disc_comp.scn_num);
            g_conn_state.remote_scn = param->disc_comp.scn[0];
            ESP_LOGI(SPP_TAG, "scn:%d service_name:%s", param->disc_comp.scn[0], param->disc_comp.service_name[0]);
            // for (i = 0; i < param->disc_comp.scn_num; i++) {
            //     ESP_LOGI(SPP_TAG, "-- [%d] scn:%d service_name:%s", i, param->disc_comp.scn[i],
            //              param->disc_comp.service_name[i]);
            // }
            // /* We only connect to the first found server on the remote SPP acceptor here */
            // esp_spp_connect(sec_mask, role_master, param->disc_comp.scn[0], peer_bd_addr);
        } else {
            ESP_LOGE(SPP_TAG, "ESP_SPP_DISCOVERY_COMP_EVT status=%d", param->disc_comp.status);
        }
        break;
    case ESP_SPP_OPEN_EVT:
        if (param->open.status == ESP_SPP_SUCCESS) {
            ESP_LOGI(SPP_TAG, "ESP_SPP_OPEN_EVT handle:%"PRIu32" rem_bda:[%s]", param->open.handle,
                     bda2str(param->open.rem_bda, bda_str, sizeof(bda_str)));
            
            xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
            g_spp_handle = param->open.handle;
            g_spp_connected = true;
            g_spp_congested = false;
            // 重置统计数据
            g_packets_sent = 0;
            g_packets_dropped = 0;
            g_bytes_sent = 0;
            xSemaphoreGive(g_spp_mutex);
            ESP_LOGI(SPP_TAG, "SPP connection opened");

            // 连接成功后关闭扫描模式
            esp_err_t ret = esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
            if (ret != ESP_OK) {
                ESP_LOGW(SPP_TAG, "Failed to disable scan mode: %s", 
                esp_err_to_name(ret));
            } else {
                ESP_LOGI(SPP_TAG, "Scan mode disabled to save power");
            }
        } else {
            ESP_LOGE(SPP_TAG, "ESP_SPP_OPEN_EVT status:%d", param->open.status);
        }
        break;
    case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT status:%d handle:%"PRIu32" close_by_remote:%d", param->close.status,
                 param->close.handle, param->close.async);
        xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
        g_spp_connected = false;
        g_spp_handle = 0;
        ESP_LOGI(SPP_TAG, "SPP connection closed. Stats: sent=%lu dropped=%lu bytes=%lu",
                g_packets_sent, g_packets_dropped, g_bytes_sent);
        xSemaphoreGive(g_spp_mutex);
        break;
    case ESP_SPP_START_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_START_EVT");
        break;
    case ESP_SPP_CL_INIT_EVT:
        if (param->cl_init.status == ESP_SPP_SUCCESS) {
            ESP_LOGI(SPP_TAG, "ESP_SPP_CL_INIT_EVT handle:%"PRIu32" sec_id:%d", param->cl_init.handle, param->cl_init.sec_id);
        } else {
            ESP_LOGE(SPP_TAG, "ESP_SPP_CL_INIT_EVT status:%d", param->cl_init.status);
        }
        break;
    case ESP_SPP_DATA_IND_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_DATA_IND_EVT");
        break;
    case ESP_SPP_WRITE_EVT:
        xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
        g_spp_congested = param->write.cong;
        g_bytes_sent += param->write.len;
        xSemaphoreGive(g_spp_mutex);
        break;
    case ESP_SPP_CONG_EVT:
        xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
        g_spp_congested = param->cong.cong;
        xSemaphoreGive(g_spp_mutex);
        ESP_LOGD(SPP_TAG, "SPP congestion: %d", param->cong.cong);
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_OPEN_EVT");
        break;
    case ESP_SPP_UNINIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_UNINIT_EVT");
        break;
    default:
        break;
    }
}

static bool get_name_from_eir(uint8_t *eir, char *bdname, uint8_t *bdname_len)
{
    uint8_t *rmt_bdname = NULL;
    uint8_t rmt_bdname_len = 0;

    if (!eir) {
        return false;
    }

    rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME, &rmt_bdname_len);
    if (!rmt_bdname) {
        rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_SHORT_LOCAL_NAME, &rmt_bdname_len);
    }

    if (rmt_bdname) {
        if (rmt_bdname_len > ESP_BT_GAP_MAX_BDNAME_LEN) {
            rmt_bdname_len = ESP_BT_GAP_MAX_BDNAME_LEN;
        }

        if (bdname) {
            memcpy(bdname, rmt_bdname, rmt_bdname_len);
            bdname[rmt_bdname_len] = '\0';
        }
        if (bdname_len) {
            *bdname_len = rmt_bdname_len;
        }
        return true;
    }

    return false;
}

static void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch(event){
    case ESP_BT_GAP_DISC_RES_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_DISC_RES_EVT");
        ESP_LOG_BUFFER_HEX(SPP_TAG, param->disc_res.bda, ESP_BD_ADDR_LEN);
        /* Find the target peer device name in the EIR data */
        for (int i = 0; i < param->disc_res.num_prop; i++){
            if (param->disc_res.prop[i].type == ESP_BT_GAP_DEV_PROP_EIR && get_name_from_eir(param->disc_res.prop[i].val, peer_bdname, &peer_bdname_len)){
                ESP_LOG_BUFFER_CHAR(SPP_TAG, peer_bdname, peer_bdname_len);
                if (strlen(remote_device_name) == peer_bdname_len && strncmp(peer_bdname, remote_device_name, peer_bdname_len) == 0) {
                    g_conn_state.device_found = true;
                    memcpy(g_conn_state.target_addr, param->disc_res.bda, ESP_BD_ADDR_LEN);
                    esp_bt_gap_cancel_discovery();
                }
            }
        }
        break;
    case ESP_BT_GAP_DISC_STATE_CHANGED_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_DISC_STATE_CHANGED_EVT");
        if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
            g_conn_state.discovery_completed = true;
            xSemaphoreGive(g_conn_state.discovery_sem);
        }
        break;
    case ESP_BT_GAP_RMT_SRVCS_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_RMT_SRVCS_EVT");
        break;
    case ESP_BT_GAP_RMT_SRVC_REC_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_RMT_SRVC_REC_EVT");
        break;
    case ESP_BT_GAP_AUTH_CMPL_EVT:{
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
            ESP_LOGI(SPP_TAG, "authentication success: %s", param->auth_cmpl.device_name);
            ESP_LOG_BUFFER_HEX(SPP_TAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
        } else {
            ESP_LOGE(SPP_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }
    case ESP_BT_GAP_PIN_REQ_EVT:{
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
        if (param->pin_req.min_16_digit) {
            ESP_LOGI(SPP_TAG, "Input pin code: 0000 0000 0000 0000");
            esp_bt_pin_code_t pin_code = {0};
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
        } else {
            ESP_LOGI(SPP_TAG, "Input pin code: 1234");
            esp_bt_pin_code_t pin_code;
            pin_code[0] = '1';
            pin_code[1] = '2';
            pin_code[2] = '3';
            pin_code[3] = '4';
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
        }
        break;
    }

#if (CONFIG_EXAMPLE_SSP_ENABLED == true)
    case ESP_BT_GAP_CFM_REQ_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %"PRIu32, param->cfm_req.num_val);
        ESP_LOGW(SPP_TAG, "To confirm the value, type `spp ok;`");
        break;
    case ESP_BT_GAP_KEY_NOTIF_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%"PRIu32, param->key_notif.passkey);
        ESP_LOGW(SPP_TAG, "Waiting response...");
        break;
    case ESP_BT_GAP_KEY_REQ_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        ESP_LOGW(SPP_TAG, "To input the key, type `spp key xxxxxx;`");
        break;
#endif

    case ESP_BT_GAP_MODE_CHG_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_MODE_CHG_EVT mode:%d", param->mode_chg.mode);
        break;

    default:
        break;
    }
}

esp_err_t connect_spp_device(void) {
    esp_err_t ret;

    // 检查是否已经连接
    xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
    if (g_spp_connected) {
        xSemaphoreGive(g_spp_mutex);
        return ESP_OK;
    }
    xSemaphoreGive(g_spp_mutex);

    // 检查重试次数
    if (g_conn_state.retry_count >= MAX_RETRY_COUNT) {
        ESP_LOGE(SPP_TAG, "Max retry count reached");
        g_conn_state.retry_count = 0;
        return ESP_FAIL;
    }

    // 创建信号量（如果还没创建）
    if (g_conn_state.discovery_sem == NULL) {
        g_conn_state.discovery_sem = xSemaphoreCreateBinary();
        if (g_conn_state.discovery_sem == NULL) {
            ESP_LOGE(SPP_TAG, "Failed to create semaphore");
            return ESP_ERR_NO_MEM;
        }
    }

    // 重置状态
    g_conn_state.device_found = false;
    g_conn_state.discovery_completed = false;

    // 开始设备发现
    ESP_LOGI(SPP_TAG, "Starting device discovery...");
    ret = esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    if (ret != ESP_OK) {
        ESP_LOGE(SPP_TAG, "Set scan mode failed: %s", esp_err_to_name(ret));
        return ret;
    }
    ret = esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(SPP_TAG, "Start discovery failed: %s", esp_err_to_name(ret));
        return ret;
    }

    // 等待发现完成或超时
    if (xSemaphoreTake(g_conn_state.discovery_sem, pdMS_TO_TICKS(DISCOVERY_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(SPP_TAG, "Discovery timeout");
        esp_bt_gap_cancel_discovery();
        g_conn_state.retry_count++;
        return ESP_ERR_TIMEOUT;
    }

    // 检查是否找到目标设备
    if (!g_conn_state.device_found) {
        ESP_LOGE(SPP_TAG, "Target device not found");
        g_conn_state.retry_count++;
        return ESP_ERR_NOT_FOUND;
    }

    // 尝试建立SPP连接
    ESP_LOGI(SPP_TAG, "Initiating SPP connection...");
    ret = esp_spp_connect(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_MASTER, g_conn_state.remote_scn, g_conn_state.target_addr);
    if (ret != ESP_OK) {
        ESP_LOGE(SPP_TAG, "SPP connect failed: %s", esp_err_to_name(ret));
        g_conn_state.retry_count++;
        return ret;
    }

    // 重置重试计数
    g_conn_state.retry_count = 0;
    return ESP_OK;
}

static inline int min(int a, int b) { return a < b ? a : b; }

static esp_err_t send_large_packet(uint32_t handle, uint8_t *data, size_t len) {
    size_t sent = 0;
    while (sent < len) {
        size_t chunk_size = min(SPP_MTU_SIZE, len - sent);
        
        // 等待直到不拥塞
        while (1) {
            xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
            bool can_send = !g_spp_congested && g_spp_connected;
            xSemaphoreGive(g_spp_mutex);
            
            if (can_send) break;
            if (!g_spp_connected) return ESP_FAIL;
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        esp_err_t err = esp_spp_write(handle, chunk_size, data + sent);
        if (err != ESP_OK) {
            return err;
        }
        sent += chunk_size;
    }
    return ESP_OK;
}

// SPP发送任务
void spp_send_task(void *pvParameters) {
    spp_data_packet_t packet;
    TickType_t current_tick;
    TickType_t last_connection_attempt = 0;
    const TickType_t connection_retry_interval = pdMS_TO_TICKS(5000); // 5秒重试间隔

    while (1) {
        xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
        bool is_connected = g_spp_connected;
        xSemaphoreGive(g_spp_mutex);

        if (!is_connected) {
            TickType_t now = xTaskGetTickCount();
            if ((now - last_connection_attempt) >= connection_retry_interval) {
                ESP_LOGI(SPP_TAG, "Attempting to reconnect...");
                connect_spp_device();
                last_connection_attempt = now;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        if (xQueueReceive(g_spp_queue, &packet, pdMS_TO_TICKS(100)) == pdTRUE) {
            bool can_send = false;
            current_tick = xTaskGetTickCount();
            
            // 检查数据包是否超时
            if ((current_tick - packet.timestamp) > pdMS_TO_TICKS(SPP_PACKET_TIMEOUT_MS)) {
                ESP_LOGW(SPP_TAG, "Packet timeout, dropping (type=%d, size=%d)",
                         packet.type, packet.len);
                free(packet.data);
                xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
                g_packets_dropped++;
                xSemaphoreGive(g_spp_mutex);
                continue;
            }

            // 等待直到可以发送
            do {
                xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
                can_send = g_spp_connected && !g_spp_congested;
                if (!g_spp_connected) {
                    free(packet.data);
                    g_packets_dropped++;
                    xSemaphoreGive(g_spp_mutex);
                    break;
                }
                xSemaphoreGive(g_spp_mutex);
                
                if (!can_send) {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            } while (!can_send);

            if (can_send) {
                esp_err_t err;
                if (packet.len > SPP_MTU_SIZE) {
                    err = send_large_packet(g_spp_handle, packet.data, packet.len);
                } else {
                    err = esp_spp_write(g_spp_handle, packet.len, packet.data);
                }

                xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
                if (err == ESP_OK) {
                    g_packets_sent++;
                } else {
                    g_packets_dropped++;
                    ESP_LOGE(SPP_TAG, "Failed to send: %s", esp_err_to_name(err));
                }
                xSemaphoreGive(g_spp_mutex);
            }

            free(packet.data);
        }
    }
}


esp_err_t send_image_data(uint8_t *data, size_t length, spp_data_type_t type) {
    if (!data || length == 0 || 
        (type == SPP_DATA_TYPE_HD && length > MAX_HD_PACKET_SIZE) ||
        (type == SPP_DATA_TYPE_LD && length > MAX_LD_PACKET_SIZE)) {
        return ESP_ERR_INVALID_ARG;
    }

    // 检查连接状态
    bool connected = false;
    xSemaphoreTake(g_spp_mutex, portMAX_DELAY);
    connected = g_spp_connected;
    xSemaphoreGive(g_spp_mutex);

    if (!connected) {
        return ESP_ERR_INVALID_STATE;
    }

    // 为图像数据分配内存
    uint8_t *data_copy = malloc(length);
    if (!data_copy) {
        return ESP_ERR_NO_MEM;
    }
    memcpy(data_copy, data, length);

    // 创建数据包
    spp_data_packet_t packet = {
        .data = data_copy,
        .len = length,
        .type = type,
        .timestamp = xTaskGetTickCount()
    };

    // 发送到队列，使用不同的超时时间
    TickType_t timeout = (type == SPP_DATA_TYPE_HD) ? 
                        pdMS_TO_TICKS(1000) : pdMS_TO_TICKS(100);
    
    if (xQueueSend(g_spp_queue, &packet, timeout) != pdTRUE) {
        free(data_copy);
        return ESP_ERR_TIMEOUT;
    }

    return ESP_OK;
}


// 初始化SPP发送机制
esp_err_t init_spp_sender(void) {
    // 创建队列
    g_spp_queue = xQueueCreate(SPP_QUEUE_SIZE, sizeof(spp_data_packet_t));
    if (g_spp_queue == NULL) {
        return ESP_ERR_NO_MEM;
    }

    // 创建互斥锁
    g_spp_mutex = xSemaphoreCreateMutex();
    if (g_spp_mutex == NULL) {
        vQueueDelete(g_spp_queue);
        return ESP_ERR_NO_MEM;
    }

    // 创建发送任务
    BaseType_t ret = xTaskCreate(spp_send_task, "spp_send_task", 4096, NULL, 5, NULL);
    if (ret != pdPASS) {
        vQueueDelete(g_spp_queue);
        vSemaphoreDelete(g_spp_mutex);
        return ESP_ERR_NO_MEM;
    }

    // 尝试首次连接
    ret = connect_spp_device();
    if (ret != ESP_OK) {
        ESP_LOGW(SPP_TAG, "Initial connection attempt failed: %s", esp_err_to_name(ret));
        // 不返回错误，让任务继续尝试连接
    }

    return ESP_OK;
}

void initialize_spp(void)
{
    esp_err_t ret = ESP_OK;
    char bda_str[18] = {0};

    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();

    if ((ret = esp_bluedroid_init_with_cfg(&bluedroid_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable bluedroid failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s gap register failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp register failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    esp_spp_cfg_t bt_spp_cfg = {
        .mode = esp_spp_mode,
        .enable_l2cap_ertm = esp_spp_enable_l2cap_ertm,
        .tx_buffer_size = 0, /* Only used for ESP_SPP_MODE_VFS mode */
    };
    if ((ret = esp_spp_enhanced_init(&bt_spp_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp init failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = init_spp_sender()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s init spp sender failed : %s", __func__, esp_err_to_name(ret));
    }

    ESP_LOGI(SPP_TAG, "Own address:[%s]", bda2str((uint8_t *)esp_bt_dev_get_address(), bda_str, sizeof(bda_str)));
}