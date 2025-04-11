/**
 * ESP32-CAM OV2640 Camera Pin Configuration
 * Verified against hardware schematic and ESP32-CAM standard pinouts
 *
 * Power/Control Pins:
 * - PWDN (Power Down): GPIO32 (Active LOW)
 * - RESET: Not Connected (NC)
 * - XCLK (System Clock): GPIO0 (20MHz output)
 *
 * I2C (SCCB) Interface:
 * - SDA (I2C Data): GPIO26
 * - SCL (I2C Clock): GPIO27
 *
 * Parallel Data Bus:
 * - D0: GPIO5  (LSB)
 * - D1: GPIO18
 * - D2: GPIO19
 * - D3: GPIO21
 * - D4: GPIO36
 * - D5: GPIO39
 * - D6: GPIO34
 * - D7: GPIO35  (MSB)
 *
 * Timing/Sync Pins:
 * - VSYNC (Vertical Sync): GPIO25
 * - HREF (Horizontal Reference): GPIO23
 * - PCLK (Pixel Clock): GPIO22 (Input)
 *
 * Note:
 * - GPIO16/17 reserved for PSRAM (must not be reassigned)
 * - GPIO0 must remain stable during boot (no other functions)
 * - All pin mappings confirmed against ESP32-CAM reference design
 */

#include "esp_camera.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "CAMERA_MAIN"

void app_main(void) {
    ESP_LOGI(TAG, "Starting camera application");

    // Camera configuration structure with verified pin mappings
    camera_config_t camera_config = {
        .pin_pwdn     = 32,  // GPIO32 for power down
        .pin_reset    = -1,  // NC (not connected)
        .pin_xclk     = 0,   // GPIO0 for XCLK (20MHz)
        .pin_sccb_sda = 26,  // GPIO26 for I2C SDA
        .pin_sccb_scl = 27,  // GPIO27 for I2C SCL

        // Data bus pins (D0-D7)
        .pin_d7 = 35,
        .pin_d6 = 34,
        .pin_d5 = 39,
        .pin_d4 = 36,
        .pin_d3 = 21,
        .pin_d2 = 19,
        .pin_d1 = 18,
        .pin_d0 = 5,

        // Timing/sync pins
        .pin_vsync = 25,  // GPIO25
        .pin_href  = 23,  // GPIO23
        .pin_pclk  = 22,  // GPIO22

        // Camera parameters
        .xclk_freq_hz = 20000000,  // 20MHz XCLK frequency
        .ledc_timer   = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

        .pixel_format = PIXFORMAT_JPEG,         // JPEG format for better performance
        .frame_size   = FRAMESIZE_SVGA,         // 800x600 resolution
        .jpeg_quality = 12,                     // Quality (0-63, lower is better)
        .fb_count     = 1,                      // Single frame buffer
        .grab_mode    = CAMERA_GRAB_WHEN_EMPTY  // Grab when buffer is empty
    };

    // Initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera initialization failed with error 0x%x", err);
        return;
    }

    ESP_LOGI(TAG, "Camera initialized successfully");

    // FreeRTOS task for timed photo capture
    void capture_task(void *pvParameters) {
        ESP_LOGI(TAG, "Starting capture task");

        while (true) {
            // Capture a frame from camera
            camera_fb_t *fb = esp_camera_fb_get();
            if (!fb) {
                ESP_LOGE(TAG, "Failed to capture frame");
            } else {
                ESP_LOGI(TAG, "Frame captured! Width: %d, Height: %d, Size: %d bytes",
                         fb->width, fb->height, fb->len);

                // Process frame here if needed (e.g., save to SD card, transmit over WiFi)

                // Return the frame buffer back to the driver for reuse
                esp_camera_fb_return(fb);
            }

            // Wait exactly 5 seconds before next capture
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }

    // Create the capture task
    xTaskCreate(
        capture_task,           // Task function
        "camera_capture_task",  // Task name
        4096,                   // Stack size (bytes)
        NULL,                   // Task parameters
        5,                      // Task priority (higher number = higher priority)
        NULL                    // Task handle
    );

    ESP_LOGI(TAG, "Capture task created successfully");

    // Main application loop (kept empty as all work is done in tasks)
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
