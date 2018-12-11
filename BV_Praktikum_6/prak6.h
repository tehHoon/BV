#ifndef PRAK6_H
#define PRAK6_H

#include <opencv2/opencv.hpp>
#include <QtMath>
#include <string.h>
#include <iostream>

class prak6
{
private:
    double bThreshMin = 190.0;
    double bThreshMax = 255.0;

    cv::Mat src, imgGray,
        resultBackground,
        resultSubtraction,
        resultBinary,
        resultOpening,
        resultClosing,
        resultOverlapping;

public:
    prak6();
    /* Hilfsfunktionen */
    void stretch(double &g, double wMin, double wMax, double gMin, double gMax, double gamma=1);
    cv::Mat stretchMat(cv::Mat src, double wMin, double wMax, double gamma=1);
    std::string getType(cv::Mat src);
    void printMat(cv::Mat src);
    void imgshow(cv::Mat src, int width, int height, int posX, int posY, std::string winName);

    /* Praktikumsaufgaben */
    void getBackgroundImage(cv::Mat src);

    void setBThreshMin(double min);
    void setBThreshMax(double max);
    void binaryImage();

    void Opening();
    void Closing();

    void Overlap();

    void printAllResults(cv::Mat src);
};

#endif // PRAK6_H
