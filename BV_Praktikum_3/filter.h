#ifndef FILTER_H
#define FILTER_H

#include <opencv2/opencv.hpp>
#include <QtMath>
#include <QDebug>

class filter
{
private:
    cv::Size kSize = cv::Size(1,1);
    double sigma = 0.5;
    double cannyMin = 50.0;
    double cannyMax = 100.0;
    cv::Mat filtered;

public:
    filter();
    ~filter();

    void stretch(double &g, double wMin, double wMax, double gMin, double gMax, double gamma=1);
    cv::Mat stretchMat(cv::Mat src, double wMin, double wMax, double gamma=1);
    void setKernelSize(int value);
    void setCannyMax(double value);
    void setCannyMin(double value);

    //methoden aufg 1
    cv::Mat falten(cv::Mat filter, cv::Mat src);

    //methoden aufg 2a
    cv::Mat normMittelFilter(cv::Mat src);
    cv::Mat gauss(cv::Mat src);

    //methoden aufg 2b
    cv::Mat xDeriv(cv::Mat src);
    cv::Mat yDeriv(cv::Mat src);
    cv::Mat laPlace(cv::Mat src);
    cv::Mat sobelX(cv::Mat src);
    cv::Mat sobelY(cv::Mat src);
    cv::Mat sobel(cv::Mat src);
    cv::Mat canny(cv::Mat src);
};

#endif // FILTER_H
