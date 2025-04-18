#include "camera_config.h"
#include "esp_log.h"
#include "image_capture.h"
#include "spp.h"
#include <string.h>

#define MAIN_TAG "MAIN_APP"

void app_main(void) {
    // Initialize camera with default 128x128 grayscale configuration
    esp_err_t camera_init_result = initialize_camera();
    if (camera_init_result != ESP_OK) {
        ESP_LOGE(MAIN_TAG, "Camera initialization failed! Error: 0x%x", camera_init_result);
        return;  // Critical failure - cannot proceed without camera
    }
    ESP_LOGI(MAIN_TAG, "Camera initialized successfully in 128x128 grayscale mode");

    initialize_spp();

    // Start the periodic image capture task (250ms interval)
    initialize_image_capture();
    ESP_LOGI(MAIN_TAG, "Image capture task started");


    // Main infinite loop - FreeRTOS tasks handle all functionality
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));            // Sleep 1 second (prevents watchdog triggers)
        ESP_LOGV(MAIN_TAG, "Main loop heartbeat");  // Verbose logging for debugging
        // char * data = "hello from esp32 camera!";
        // esp_err_t ret = send_image_data((uint8_t *)data, strlen(data), SPP_DATA_TYPE_LD);
        // if (ret != ESP_OK){
        //     ESP_LOGW(MAIN_TAG, "send data error!");
        // }
    }
}
