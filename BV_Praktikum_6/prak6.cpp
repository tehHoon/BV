#include "prak6.h"

prak6::prak6()
{

}
/* Hilfsfunktionen */
void prak6::stretch(double &g, double wMin, double wMax, double gMin, double gMax, double gamma){
    g = (wMax-wMin)* qPow(((g-gMin)/(gMax-gMin)),gamma) + wMin;
}

cv::Mat prak6::stretchMat(cv::Mat src, double wMin, double wMax, double gamma){
    double pixel, min, max;
    cv::minMaxLoc(src, &min, &max);
    std::cout << min << " " << max << std::endl;

    //falls das min/max nicht in dem [0;255] bereich liegt
    if(min != wMin && max != wMax){
        for(int m = 0; m<src.rows; m++){
            for(int n = 0; n<src.cols; n++){
                pixel = src.at<double>(n,m);
                stretch(pixel, wMin, wMax, min, max, gamma);
                src.at<double>(n,m) = pixel;
            }
        }
    }
    return src;
}

std::string prak6::getType(cv::Mat src){
    std::string r;
    uchar depth = src.type() & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (src.type() >> CV_CN_SHIFT);
    switch (depth) {
        case CV_8U:     r="8U";     break;
        case CV_8S:     r="8S";     break;
        case CV_16U:    r="16U";    break;
        case CV_16S:    r="16S";    break;
        case CV_32S:    r="32S";    break;
        case CV_32F:    r="32F";    break;
        case CV_64F:    r="64F";    break;
        default:        r="User";   break;
    }

    r+="C";
    r+=(chans+'0');
    return r;
}

void prak6::printMat(cv::Mat src){
    std::cout << src << " = " << std::endl << " " << src << std::endl << std::endl;
}

void prak6::imgshow(cv::Mat src, int width, int height, int posX, int posY, std::string winName){
    namedWindow(winName, CV_WINDOW_FREERATIO);
    resizeWindow(winName, width, height);
    moveWindow(winName, posX, posY);
    imshow(winName, src);
}
/* Hilfsfunktionen Ende */

/* Praktikumsaufgaben */
void prak6::getBackgroundImage(cv::Mat src){
    //grayscale floats
    this->src = src;
    Mat background, original;
    cvtColor(src, imgGray, CV_BGR2GRAY);
    imgGray.convertTo(background, CV_32F);
    imgGray.convertTo(original, CV_32F);
    normalize(background, background, 0, 1, NORM_MINMAX);

    //kernel erstellen und hintergrund filtern
    Mat kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(23,23));
    morphologyEx(background, background, MORPH_OPEN, kernel);
    morphologyEx(background, background, MORPH_CLOSE, kernel);

    imgshow(background, 640, 490, 0, 0, "Hintergrund");
    resultBackground = background;


    //Subtraktion mit Hintergrund
    //Mat sub = original - filtered;
    Mat sub(src.rows, src.cols, CV_32F);
    cv::subtract(original, background, sub, Mat());
    normalize(sub, sub, 0, 1, NORM_MINMAX);

    imgshow(sub, 640, 490, 640, 0, "Subtraktion mit Hintergrund");
    resultSubtraction = sub;
}

void prak6::setBThreshMin(double min){
    bThreshMin = min;
}

void prak6::setBThreshMax(double max){
    bThreshMax = max;
}

void prak6::binaryImage(){
    cv::Mat bImg;

    Mat sub;
    normalize(resultSubtraction, sub, 0, 255, NORM_MINMAX);
    sub.convertTo(sub, CV_8U);

    threshold(sub, bImg, bThreshMin, bThreshMax, 0);

    imgshow(bImg, 640, 490, 1280, 0, "Binaerbild");
    resultBinary = bImg;
}

void prak6::Opening(){
    Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(3,3));

    Mat bImg = resultBinary;
    morphologyEx(bImg, bImg, MORPH_OPEN, kernel);

    imgshow(bImg, 640, 490, 0, 522, "Opening");
    resultOpening = bImg;
}

void prak6::Closing(){
    Mat kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(4,4));

    Mat bImg = resultOpening;
    morphologyEx(bImg, bImg, MORPH_CLOSE, kernel);

    imgshow(bImg, 640, 490, 640, 522, "Closing");
    resultClosing = bImg;
}

void prak6::Overlap(){
    Mat binInverted;
    bitwise_not(resultClosing, binInverted);
    Mat overlap = binInverted + imgGray;

    imgshow(overlap, 640, 490, 1280, 522, "Ueberlagerung");
    resultOverlapping = overlap;
}

void prak6::printAllResults(cv::Mat src){
    getBackgroundImage(src);
    binaryImage();
    Opening();
    Closing();
    Overlap();
}
