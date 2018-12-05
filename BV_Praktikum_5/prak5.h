#ifndef PRAK5_H
#define PRAK5_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

class prak5
{
private:
    double minHessian = 2000;
    double minDistance = 0.01;
    std::vector<cv::DMatch> good_matches;
    std::vector<cv::Point2f> points1;
    std::vector<cv::Point2f> points2;
    cv::Mat affineTransform;
public:
    prak5();
    void findFeatures(cv::Mat src, cv::Mat processed);
    void setMinHessian(double value);
    void setMinDistance(double value);
    void getTransformation(cv::Mat src, cv::Mat processed);

    cv::Mat calcOriginal(cv::Mat src, cv::Mat processed);
};

#endif // PRAK5_H
