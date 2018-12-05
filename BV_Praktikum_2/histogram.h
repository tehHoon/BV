#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QtMath>
#include <QDebug>
#include <opencv2/opencv.hpp>

class histogram
{
public:
    histogram();
    cv::Mat createHistogram(cv::Mat src);
    void stretch(double &g, double wMin, double wMax, double gMin, double gMax, double gamma = 1);
    cv::Mat stretchHistogram(cv::Mat src, double gamma = 1);
    cv::Mat linearHistogram(cv::Mat src);
    cv::Mat accumulateHistogram(cv::Mat src);
};

#endif // HISTOGRAM_H
