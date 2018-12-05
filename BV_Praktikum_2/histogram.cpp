#include "histogram.h"

histogram::histogram(){}

void histogram::stretch(double &g, double wMin, double wMax, double gMin, double gMax, double gamma){
    g = (wMax-wMin)* qPow(((g-gMin)/(gMax-gMin)),gamma) + wMin;
}

cv::Mat histogram::createHistogram(cv::Mat src){
    cv::Mat hist;

    // Establish the number of bins
    int histSize = 256;

    //set range
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    //compute histogram
    cv::calcHist(&src, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    // Draw the histogram
    int hist_w = 512;
    int hist_h = 512;
    int bin_w = cvRound( (double) hist_w/histSize );

    cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar(0,0,0) );

    // Normalize the result to [ 0, histImage.rows ]
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

    for(int i = 1; i < histSize; i++ ){
        cv::line( histImage, cv::Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ) ,
                           cv::Point( bin_w*(i), hist_h ),
                           cv::Scalar( 255, 255, 255), 1, 8, 0 );
    }

    return histImage;
}

cv::Mat histogram::stretchHistogram(cv::Mat src, double gamma){
    cv::Mat output(src.rows, src.cols, CV_8U);

    double pixel, min, max;
    cv::minMaxLoc(src, &min, &max);

    for(int m = 0; m<src.rows; m++){
        for(int n = 0; n<src.cols; n++){
            pixel = src.at<uchar>(cv::Point(n,m));

            //Histogramm-Stretching
            stretch(pixel, 0, 255, min, max, gamma);

            output.at<uchar>(cv::Point(n,m)) = pixel;
        }
    }

    return output;
}

cv::Mat histogram::linearHistogram(cv::Mat src){
    cv::Mat hist(src.rows, src.cols, CV_8U);
    cv::equalizeHist(src, hist);
    return hist;
}

cv::Mat histogram::accumulateHistogram(cv::Mat src){
    int histSize = 256;
    cv::Mat accumulatedHist(src.rows, src.cols, CV_8U);
    accumulatedHist = src.clone();
    for (int i = 1; i < histSize; i++) {
        accumulatedHist.at<float>(i) += accumulatedHist.at<float>(i - 1);
    }
    return accumulatedHist;
}
