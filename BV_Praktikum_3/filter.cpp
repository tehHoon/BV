#include "filter.h"

filter::filter(){}
filter::~filter(){}

void filter::stretch(double &g, double wMin, double wMax, double gMin, double gMax, double gamma){
    g = (wMax-wMin)* qPow(((g-gMin)/(gMax-gMin)),gamma) + wMin;
}

cv::Mat filter::stretchMat(cv::Mat src, double wMin, double wMax, double gamma){
    double pixel, min, max;
    cv::minMaxLoc(src, &min, &max);
    std::cout << "min: " << min << " max: " << max << std::endl;

    if(min != wMin && max != wMax) {
        //falls das min/max nicht in dem [0;255] bereich liegt
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

void filter::setKernelSize(int value) {
    this->kSize = cv::Size(value,value);
    sigma = 0.3 * ((value-1)*0.5 - 1) + 0.8;
    //qDebug() << "Sigma: " << sigma;
}

void filter::setCannyMax(double value){
    cannyMax = value;
}

void filter::setCannyMin(double value){
    cannyMin = value;
}

//Methoden Aufg 1
cv::Mat filter::falten(cv::Mat filter, cv::Mat src){
    cv::Mat img_src;
    cv::Mat img_dst = src.clone();

    std::cout << "filter = " << std::endl << " " << filter << std::endl << std::endl;

    //Bild mit einem Rand aus Nullen versehen
    cv::copyMakeBorder(src,img_src,1,1,1,1,cv::BORDER_CONSTANT,cv::Scalar(0));

    //Bilder in Datentyp double umwandeln
    img_src.convertTo(img_src, CV_64F);
    img_dst.convertTo(img_dst, CV_64F);
    filter.convertTo(filter, CV_64F);

    //Filter wird um 180 Grad gedreht
    cv::Mat kernel;
    cv::flip(filter, kernel, -1);

    //über jeden Punkt in der Matrix gehen
    img_dst.forEach<double>([kernel, img_src](double &pixel, const int* pos)->void{
        pixel = cv::sum(img_src(cv::Rect(pos[1],pos[0],3,3)).mul(kernel))[0];
    });

    //Histogrammstretching
    img_dst = stretchMat(img_dst, 0, 255);

    //Bild zurück wandeln
    img_dst.convertTo(img_dst, CV_8U);

    //filter2D zum vergleich
    cv::Mat dst;
    cv::filter2D(src, dst, -1, filter);
    cv::namedWindow("filter2D OpenCV", cv::WINDOW_NORMAL);
    cv::imshow("filter2D OpenCV", dst);

    return img_dst;
}

//Methoden Aufg 2a
cv::Mat filter::normMittelFilter(cv::Mat src){
    cv::blur(src, filtered, kSize);
    return filtered;
}

cv::Mat filter::gauss(cv::Mat src){
    cv::GaussianBlur(src, filtered, kSize, sigma);
    return filtered;
}

//Methoden Aufg 2b
cv::Mat filter::xDeriv(cv::Mat src){
    filtered.convertTo(filtered, CV_64F);
    src.convertTo(src, CV_64F);
    cv::Scharr(src, filtered, -1, 1, 0);
    filtered = stretchMat(filtered, 0, 255);
    filtered.convertTo(filtered, CV_8U);
    return filtered;
}

cv::Mat filter::yDeriv(cv::Mat src){
    filtered.convertTo(filtered, CV_64F);
    src.convertTo(src, CV_64F);
    cv::Scharr(src, filtered, -1, 0, 1);
    filtered = stretchMat(filtered, 0, 255);
    filtered.convertTo(filtered, CV_8U);
    return filtered;
}

cv::Mat filter::laPlace(cv::Mat src){
    filtered.convertTo(filtered, CV_64F);
    src.convertTo(src, CV_64F);
    src = gauss(src);
    cv::Laplacian(src, filtered, -1, 1);
    filtered = stretchMat(filtered, 0, 255);
    filtered.convertTo(filtered, CV_8U);
    return filtered;
}

cv::Mat filter::sobelX(cv::Mat src){
    filtered.convertTo(filtered, CV_64F);
    src.convertTo(src, CV_64F);
    cv::Sobel(src, filtered, -1, 1, 0);
    filtered = stretchMat(filtered, 0, 255);
    filtered.convertTo(filtered, CV_8U);
    return filtered;
}

cv::Mat filter::sobelY(cv::Mat src){
    filtered.convertTo(filtered, CV_64F);
    src.convertTo(src, CV_64F);
    cv::Sobel(src, filtered, -1, 0, 1);
    filtered = stretchMat(filtered, 0, 255);
    filtered.convertTo(filtered, CV_8U);
    return filtered;
}

cv::Mat filter::sobel(cv::Mat src){
    // Gradient X
    cv::Mat Gx;
    cv::Sobel( src, Gx, -1, 1, 0);
    // Gradient Y
    cv::Mat Gy;
    cv::Sobel( src, Gy, -1, 0, 1);

    //partials zurück formatieren zu CV_8U
    cv::convertScaleAbs(Gx, Gx);
    cv::convertScaleAbs(Gy, Gy);

    /* formel will irgendwie nicht
    cv::pow(Gx, 2, Gx);
    cv::pow(Gy, 2, Gy);
    cv::sqrt((Gx+Gy), filtered);
    */

    cv::addWeighted(Gx, 0.5, Gy, 0.5, 0, filtered);
    return filtered;
}

cv::Mat filter::canny(cv::Mat src){
    src = gauss(src);
    cv::Canny(src, filtered, cannyMax, cannyMin);
    return filtered;
}
