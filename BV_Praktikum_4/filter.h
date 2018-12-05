#ifndef FILTER_H
#define FILTER_H

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <QtMath>
#include <QDebug>
#include <iostream>

class filter
{
private:
    cv::Mat shift(cv::Mat magI);
    void stretch(double &g, double wMin, double wMax, double gMin, double gMax, double gamma=1);
    cv::Mat stretchMat(cv::Mat src, double wMin, double wMax, double gamma=1);

    cv::Mat complexI;       //fourier transform Mat
    cv::Mat magnitude;      //geshiftetes spektrum
    cv::Mat ImDFT;
    cv::Mat mask;           //maske (filter)
    cv::Mat mult;           //elementweise Multiplikation
                            //(geshiftetes Spektrum und Filter)
public:
    filter();
    void fourier(cv::Mat I);
    cv::Mat computeDFT(cv::Mat I);
    cv::Mat updateMag(cv::Mat complex);
    void genFilter(double threshold = 0.5, int kSize = 1);
    void elWeiseMulti();
    cv::Mat calcInverseTransform();
};

#endif // FILTER_H
