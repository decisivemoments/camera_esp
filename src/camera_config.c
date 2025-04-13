#include "esp_camera.h"
#include "esp_log.h"

// Camera module tag for logging
#define CAMERA_TAG "CAMERA_CONFIG"

// Camera configuration structure
camera_config_t camera_config = {
    .pin_pwdn     = GPIO_NUM_32,  // Power down pin
    .pin_reset    = -1,           // Reset pin (NC - Not Connected)
    .pin_xclk     = GPIO_NUM_0,   // XCLK pin
    .pin_sccb_sda = GPIO_NUM_26,  // I2C SDA pin
    .pin_sccb_scl = GPIO_NUM_27,  // I2C SCL pin

    // Camera data pins (D0-D7)
    .pin_d7 = GPIO_NUM_35,
    .pin_d6 = GPIO_NUM_34,
    .pin_d5 = GPIO_NUM_39,
    .pin_d4 = GPIO_NUM_36,
    .pin_d3 = GPIO_NUM_21,
    .pin_d2 = GPIO_NUM_19,
    .pin_d1 = GPIO_NUM_18,
    .pin_d0 = GPIO_NUM_5,

    // Camera control pins
    .pin_vsync = GPIO_NUM_25,  // VSYNC pin
    .pin_href  = GPIO_NUM_23,  // HREF pin
    .pin_pclk  = GPIO_NUM_22,  // PCLK pin

    // XCLK frequency in Hz (20MHz for OV2640)
    .xclk_freq_hz = 20000000,

    // Default to low resolution mode (128x128 grayscale)
    .ledc_timer   = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    // Pixel format and frame size will be set dynamically
    .pixel_format = PIXFORMAT_GRAYSCALE,
    .frame_size   = FRAMESIZE_128X128,

    // JPEG quality (used when in high resolution mode)
    .jpeg_quality = 12,

    // Number of frame buffers
    .fb_count = 2};

/**
 * @brief Initialize camera with specified configuration
 * @return esp_err_t ESP_OK on success, error code on failure
 */
esp_err_t initialize_camera() {
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(CAMERA_TAG, "Camera initialization failed: 0x%x", err);
        return err;
    }
    ESP_LOGI(CAMERA_TAG, "Camera initialized successfully");

    // Set initial resolution to 128x128 grayscale
    sensor_t *s = esp_camera_sensor_get();
    if (s == NULL) {
        ESP_LOGE(CAMERA_TAG, "Failed to get camera sensor");
        return ESP_FAIL;
    }

    // Configure initial settings
    s->set_framesize(s, FRAMESIZE_128X128);
    s->set_pixformat(s, PIXFORMAT_GRAYSCALE);

    ESP_LOGI(CAMERA_TAG, "Camera configured for 128x128 grayscale mode");
    return ESP_OK;
}

/**
 * @brief Switch camera resolution mode
 * @param high_res true for 640x480 JPEG, false for 128x128 grayscale
 * @return esp_err_t ESP_OK on success, error code on failure
 */
esp_err_t switch_camera_mode(bool high_res) {
    sensor_t *s = esp_camera_sensor_get();
    if (s == NULL) {
        ESP_LOGE(CAMERA_TAG, "Failed to get camera sensor for mode switch");
        return ESP_FAIL;
    }

    if (high_res) {
        s->set_framesize(s, FRAMESIZE_VGA);
        s->set_pixformat(s, PIXFORMAT_JPEG);
        ESP_LOGI(CAMERA_TAG, "Switched to 640x480 JPEG mode");
    } else {
        s->set_framesize(s, FRAMESIZE_128X128);
        s->set_pixformat(s, PIXFORMAT_GRAYSCALE);
        ESP_LOGI(CAMERA_TAG, "Switched to 128x128 grayscale mode");
    }

    return ESP_OK;
}
