#ifndef CAMERA_CONFIG_H
#define CAMERA_CONFIG_H

#include "esp_camera.h"
#include "esp_err.h"
#include "esp_log.h"

/**
 * @file camera_config.h
 * @brief Camera configuration interface for ESP32-CAM
 *
 * Provides declarations for:
 * - Camera hardware configuration structure
 * - Camera initialization function
 * - Camera mode switching function
 */

#define CAMERA_TAG "CAMERA_CONFIG"  ///< Logging tag for camera operations

/**
 * @brief Initialize camera with default configuration
 * @return esp_err_t ESP_OK on success, error code on failure
 *
 * @note Configures camera in 128x128 grayscale mode by default
 * @warning Must be called before any capture operations
 */
esp_err_t initialize_camera(void);

/**
 * @brief Switch camera resolution mode
 * @param high_res true for 640x480 JPEG, false for 128x128 grayscale
 * @return esp_err_t ESP_OK on success, error code on failure
 *
 * @details Maintains camera initialization while switching modes to avoid
 *          hardware reinitialization overhead between captures
 */
esp_err_t switch_camera_mode(bool high_res);

#endif  // CAMERA_CONFIG_H
