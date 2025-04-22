#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
static const char* TAG = "OpticalFlowTest";

// 网格点生成：每隔 blockSize 像素采样一个特征点
std::vector<cv::Point2f> generateGridPoints(const cv::Mat& frame, int blockSize) {
    std::vector<cv::Point2f> points;
    for (int y = 0; y < frame.rows; y += blockSize) {
        for (int x = 0; x < frame.cols; x += blockSize) {
            points.emplace_back((float)x, (float)y);
        }
    }
    ESP_LOGI(TAG, "Generated %d grid points", (int)points.size());
    return points;
}

// LK光流稀疏跟踪
void calculateSparseFlow(const cv::Mat& previousMat, const cv::Mat& currentMat, const std::vector<cv::Point2f>& previousPoints, int imageSize) {
    std::vector<cv::Point2f> nextPoints;
    std::vector<uchar> status;
    std::vector<float> err;

    // 调用光流计算函数
    cv::calcOpticalFlowPyrLK(
        previousMat, currentMat,
        previousPoints, nextPoints,
        status, err
    );

    int validCount = 0;
    for (size_t i = 0; i < status.size(); i++) {
        if (status[i]) validCount++;
    }
    ESP_LOGI(TAG, "Tracked %d / %d points", validCount, (int)status.size());
}

// 随机生成灰度图像
cv::Mat generateRandomGrayImage(int size) {
    cv::Mat image(size, size, CV_8UC1);
    cv::randu(image, 0, 255);  // 随机填充
    return image;
}

// 主测试流程：类似你的 Android startOpticalFlowExperiment
void startOpticalFlowExperiment() {
    const int IMAGE_SIZE = 128;
    const int BLOCK_SIZE = 8;
    const int ITERATIONS = 50;

    // 随机生成两帧模拟图像
    cv::Mat previousMat = generateRandomGrayImage(IMAGE_SIZE);
    cv::Mat currentMat = generateRandomGrayImage(IMAGE_SIZE);

    // 初始化特征点
    std::vector<cv::Point2f> previousPoints = generateGridPoints(previousMat, BLOCK_SIZE);

    int64_t times[ITERATIONS] = {0};

    // 光流跟踪循环测试
    for (int iteration = 0; iteration < ITERATIONS; iteration++) {
        int64_t startTime = esp_timer_get_time();  // 微秒

        calculateSparseFlow(previousMat, currentMat, previousPoints, IMAGE_SIZE);

        int64_t endTime = esp_timer_get_time();  // 微秒
        times[iteration] = endTime - startTime;

        ESP_LOGI(TAG, "Iteration %d - Time taken: %.3f ms", iteration + 1, times[iteration] / 1000.0);
    }

    // 后40次的平均值统计
    int64_t totalTime = 0;
    for (int i = 10; i < ITERATIONS; i++) {
        totalTime += times[i];
    }
    double averageMs = totalTime / 40.0 / 1000.0;
    ESP_LOGI(TAG, "Average time for last 40 iterations: %.3f ms", averageMs);
}


extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting Optical Flow Test...");
    startOpticalFlowExperiment();
}
