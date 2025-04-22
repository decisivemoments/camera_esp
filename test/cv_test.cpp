#include <vector>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/video.hpp>

using namespace cv;

void test() {

  // 创建两个矩阵
  Mat A = Mat::eye(3, 3, CV_32F);   // 单位矩阵
  Mat B = Mat::ones(3, 3, CV_32F);  // 全1矩阵

  // 矩阵相加
  Mat C = A + B;

}





// 初始化光流参数
cv::TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.03);
cv::Size winSize(31, 31);


void processOpticalFlow(const cv::Mat& prevGray, const cv::Mat& gray, std::vector<cv::Point2f>& pointsPrev, std::vector<cv::Point2f>& pointsNext) {
    std::vector<uchar> status;
    std::vector<float> err;

    // 计算光流
    cv::calcOpticalFlowPyrLK(prevGray, gray, pointsPrev, pointsNext, status, err, winSize, 3, termcrit, 0, 0.001);

    // 过滤跟踪失败的点
    size_t i, k;
    for (i = k = 0; i < pointsNext.size(); i++) {
        if (!status[i])
            continue;

        pointsNext[k++] = pointsNext[i];
    }
    pointsNext.resize(k);
}

extern "C" void app_main(void) {}