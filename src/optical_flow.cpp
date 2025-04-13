#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>

#include "esp_camera.h"
#include "esp_log.h"

#define OPTICAL_FLOW_TAG "OPTICAL_FLOW"

/**
 * @brief Converts ESP32 camera frame buffer to OpenCV Mat format
 * @param frame Pointer to camera frame buffer (must be grayscale)
 * @return cv::Mat OpenCV matrix in 8UC1 format (128x128 grayscale)
 * @note Caller must ensure frame is grayscale (PIXFORMAT_GRAYSCALE)
 */
static cv::Mat camera_fb_to_mat(camera_fb_t const* frame) {
    ESP_LOGD(OPTICAL_FLOW_TAG, "Converting frame to Mat (size: %dx%d)", frame->width, frame->height);
    return cv::Mat(frame->height, frame->width, CV_8UC1, (void*)frame->buf);
}

/**
 * @brief Calculates optical flow between two consecutive frames
 * @param prev_frame Previous 128x128 grayscale frame
 * @param curr_frame Current 128x128 grayscale frame
 * @param[out] flow_x Output array for x-component flow vectors (must have 256 elements)
 * @param[out] flow_y Output array for y-component flow vectors (must have 256 elements)
 * @return esp_err_t ESP_OK on success, error code on failure
 *
 * @details
 * - Uses calcOpticalFlowPyrLK with 16x16 grid (256 points total)
 * - Outputs 512 values (256 x-components + 256 y-components)
 * - Requires both frames to be same size (128x128) and grayscale
 */
esp_err_t calc_optical_flow(camera_fb_t const* prev_frame,
                            camera_fb_t const* curr_frame,
                            float*             flow_x,
                            float*             flow_y) {
    // Input validation
    if (!prev_frame || !curr_frame || !flow_x || !flow_y) {
        ESP_LOGE(OPTICAL_FLOW_TAG, "Invalid input parameters");
        return ESP_ERR_INVALID_ARG;
    }

    // Convert frames to OpenCV format
    cv::Mat prev_mat = camera_fb_to_mat(prev_frame);
    cv::Mat curr_mat = camera_fb_to_mat(curr_frame);

    // Generate 16x16 grid points (256 total)
    std::vector<cv::Point2f> prev_points;
    prev_points.reserve(256);
    for (int y = 8; y < 128; y += 8) {      // 16 rows (128/8=16)
        for (int x = 8; x < 128; x += 8) {  // 16 columns
            prev_points.emplace_back(x, y);
        }
    }

    // Calculate optical flow
    std::vector<cv::Point2f> curr_points;
    std::vector<uchar>       status;
    std::vector<float>       err;
    cv::calcOpticalFlowPyrLK(prev_mat, curr_mat, prev_points, curr_points,
                             status, err, cv::Size(21, 21), 3);

    // Store results in output arrays
    for (size_t i = 0; i < curr_points.size(); i++) {
        flow_x[i] = curr_points[i].x - prev_points[i].x;
        flow_y[i] = curr_points[i].y - prev_points[i].y;
    }

    ESP_LOGI(OPTICAL_FLOW_TAG, "Optical flow calculated (256 points)");
    return ESP_OK;
}
