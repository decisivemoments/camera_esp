#ifndef OPTICAL_FLOW_H
#define OPTICAL_FLOW_H

#include "esp_camera.h"
#include "esp_err.h"

/**
 * @file optical_flow.h
 * @brief Optical flow calculation interface for ESP32-CAM
 *
 * Provides declarations for:
 * - Optical flow calculation between consecutive camera frames
 * - Data structures for flow vector output
 */

#define OPTICAL_FLOW_TAG "OPTICAL_FLOW"  ///< Logging tag for optical flow operations

/**
 * @brief Calculate optical flow between two consecutive frames
 * @param prev_frame Pointer to previous camera frame buffer (must be grayscale)
 * @param curr_frame Pointer to current camera frame buffer (must be grayscale)
 * @param[out] flow_x Output array for x-component flow vectors (must have 256 elements)
 * @param[out] flow_y Output array for y-component flow vectors (must have 256 elements)
 * @return esp_err_t ESP_OK on success, error code on failure
 *
 * @details
 * - Requires both frames to be 128x128 grayscale (PIXFORMAT_GRAYSCALE)
 * - Uses OpenCV's calcOpticalFlowPyrLK with 16x16 grid (256 points total)
 * - Outputs 512 values (256 x-components + 256 y-components)
 * - Typical usage: Called from image capture task when consecutive frames are available
 *
 * @warning
 * - Caller must ensure prev_frame and curr_frame are valid and same resolution
 * - Output arrays (flow_x, flow_y) must have capacity for 256 float values each
 */
esp_err_t calc_optical_flow(camera_fb_t const* prev_frame,
                            camera_fb_t const* curr_frame,
                            float*             flow_x,
                            float*             flow_y);

#endif  // OPTICAL_FLOW_H
