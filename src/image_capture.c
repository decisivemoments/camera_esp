#include "camera_config.h"
#include "esp_camera.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "spp.h"

#define CAPTURE_TAG          "IMAGE_CAPTURE"
#define LOW_RES_INTERVAL_MS  250   // 4fps (1000ms/4 = 250ms)
#define HIGH_RES_INTERVAL_MS 5000  // 5 seconds

// Global variables for tracking capture state
static uint32_t     last_high_res_capture_time = 0;
static camera_fb_t* previous_frame             = NULL;

/**
 * @brief Capture and process image frame based on timing requirements
 *
 * Implements the core logic:
 * - Checks time since last high-res capture
 * - Switches resolution (using switch_camera_mode to avoid reinit)
 * - Captures frame (esp_camera_fb_get)
 * - For high-res: Updates last capture timestamp
 * - For low-res: Stores frame for optical flow
 */
void capture_image_task() {
    while (1) {
        uint32_t current_time   = xTaskGetTickCount() * portTICK_PERIOD_MS;
        bool     needs_high_res = (current_time - last_high_res_capture_time) >= HIGH_RES_INTERVAL_MS;
        ESP_LOGI(CAPTURE_TAG, "current_time: %lu, last_high_res_capture_time: %lu, diff: %lu",
                 current_time, last_high_res_capture_time, current_time - last_high_res_capture_time);

        // Switch resolution mode (no reinitialization)
        if (switch_camera_mode(needs_high_res) != ESP_OK) {
            ESP_LOGE(CAPTURE_TAG, "Failed to switch camera mode");
            vTaskDelay(pdMS_TO_TICKS(LOW_RES_INTERVAL_MS));
            continue;
        }

        // Capture frame
        camera_fb_t* frame = esp_camera_fb_get();
        if (!frame) {
            ESP_LOGE(CAPTURE_TAG, "Frame capture failed");
            vTaskDelay(pdMS_TO_TICKS(LOW_RES_INTERVAL_MS));
            continue;
        }

        if (needs_high_res) {
            ESP_LOGI(CAPTURE_TAG,
                     "Captured high-res frame (size: %d, width: %d, height: %d)",
                     frame->len, frame->width, frame->height);
            last_high_res_capture_time = current_time;

            // TODO: Add JPEG transmission logic
            esp_err_t ret = send_image_data((uint8_t *)frame->buf, frame->len, SPP_DATA_TYPE_LD);
            if (ret != ESP_OK){
                ESP_LOGW(CAPTURE_TAG, "send data error!");
            }

            esp_camera_fb_return(frame);
        } else {
            ESP_LOGI(CAPTURE_TAG,
                     "Captured low-res frame (size: %d, width: %d, height: %d)",
                     frame->len, frame->width, frame->height);

            // // Process optical flow if previous frame exists
            if (previous_frame) {
                int data[512];
                for(int i=0;i<512;i++){
                    data[i] = i;
                }
                esp_err_t ret = send_image_data((uint8_t *)data, 512 * 4, SPP_DATA_TYPE_LD);
                if (ret != ESP_OK){
                    ESP_LOGW(CAPTURE_TAG, "send data error!");
                }
                // // Allocate arrays for flow vectors (256 points x 2 components)
                // float flow_x[256];
                // float flow_y[256];

                // // Calculate optical flow between previous and current frames
                // esp_err_t flow_result = calc_optical_flow(previous_frame, frame, flow_x, flow_y);

                // if (flow_result == ESP_OK) {
                //     ESP_LOGI(CAPTURE_TAG,
                //              "Optical flow calculated (256 points). Sample vectors: [0]=(%.2f,%.2f), [255]=(%.2f,%.2f)",
                //              flow_x[0], flow_y[0], flow_x[255], flow_y[255]);

                //     // TODO: Add flow data processing/transmission logic here
                // } else {
                //     ESP_LOGE(CAPTURE_TAG, "Optical flow calculation failed: 0x%x", flow_result);
                // }

                // Release previous frame buffer before replacing it
                esp_camera_fb_return(previous_frame);
            }
            
            previous_frame = frame;
        }

        vTaskDelay(pdMS_TO_TICKS(LOW_RES_INTERVAL_MS));
    }
}

/**
 * @brief Initialize image capture system
 *
 * Creates FreeRTOS task for periodic capture.
 * Should be called after camera initialization.
 */
void initialize_image_capture() {
    xTaskCreate(
        capture_image_task,
        "capture_task",
        4096,  // Stack size
        NULL,
        5,  // Priority
        NULL);
    ESP_LOGI(CAPTURE_TAG, "Image capture task created");
}
