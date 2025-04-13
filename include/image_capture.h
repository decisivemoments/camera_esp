#ifndef IMAGE_CAPTURE_H
#define IMAGE_CAPTURE_H

#include "esp_camera.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @file image_capture.h
 * @brief Image capture task interface for ESP32-CAM
 *
 * Provides declarations for:
 * - Image capture task initialization
 * - Capture timing constants
 * - Frame processing functions
 */

#define CAPTURE_TAG          "IMAGE_CAPTURE"  ///< Logging tag for capture operations
#define LOW_RES_INTERVAL_MS  250              ///< 4fps interval (1000ms/4)
#define HIGH_RES_INTERVAL_MS 5000             ///< 5 second high-res interval

/**
 * @brief Initialize the periodic image capture system
 *
 * Creates FreeRTOS task that:
 * - Captures frames every 250ms (4fps)
 * - Automatically switches between:
 *   - 128x128 grayscale (low-res)
 *   - 640x480 JPEG (high-res every 5s)
 * - Calculates optical flow between consecutive low-res frames
 *
 * @note Must be called after camera initialization
 * @warning Task stack size is 4096 bytes with priority 5
 */
void initialize_image_capture(void);

#endif  // IMAGE_CAPTURE_H
