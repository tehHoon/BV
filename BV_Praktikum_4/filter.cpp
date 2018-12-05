#include "filter.h"

filter::filter(){}

void filter::stretch(double &g, double wMin, double wMax, double gMin, double gMax, double gamma){
    g = (wMax-wMin)* qPow(((g-gMin)/(gMax-gMin)),gamma) + wMin;
}

cv::Mat filter::stretchMat(cv::Mat src, double wMin, double wMax, double gamma){
    double pixel, min, max;
    cv::minMaxLoc(src, &min, &max);

    //falls das min/max nicht in dem [0;255] bereich liegt
    for(int m = 0; m<src.rows; m++){
        for(int n = 0; n<src.cols; n++){
            pixel = src.at<double>(n,m);
            stretch(pixel, wMin, wMax, min, max, gamma);
            src.at<double>(n,m) = pixel;
        }
    }
    return src;
}

cv::Mat filter::shift(cv::Mat magI){
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    cv::Mat tmp;
    // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    // swap quadrant (Top-Right with Bottom-Left)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
    return magI;
}

void filter::fourier(cv::Mat I){
    cv::cvtColor(I, I, CV_BGR2GRAY);

    //compute DFT
    complexI = computeDFT(I);
    //--compute DFT

    //show DFT
    magnitude = updateMag(complexI);
    //--show DFT

    imshow("spectrum magnitude", magnitude);
}

cv::Mat filter::computeDFT(cv::Mat I){
    //expand input image to optimal size
    cv::Mat padded;
    int m = cv::getOptimalDFTSize( I.rows );
    int n = cv::getOptimalDFTSize( I.cols );

    // on the border add zero values
    cv::copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};

    // Add to the expanded another plane with zeros
    cv::Mat complex;
    cv::merge(planes, 2, complex);

    // fourier transform
    cv::dft(complex, complex);
    //cv::imshow("dft", complex);
    return complex;
}

cv::Mat filter::updateMag(cv::Mat complex){
    cv::Mat magI;
    cv::Mat planes[] = {cv::Mat::zeros(complex.size(), CV_32F),
                       cv::Mat::zeros(complex.size(), CV_32F)};
    cv::split(complex, planes);     // planes[0] = Re(DFT(I))
                                    // planes[1] = Im(DFT(I))

    // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    cv::magnitude(planes[0], planes[1], magI);
    ImDFT = planes[1];

    // switch to logarithmic scale: log(1+magnitude)
    magI += cv::Scalar::all(1);
    log(magI, magI);

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    magI = shift(magI);

    cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX);   // Transform the matrix with float values into a
                                                        // viewable image form (float between values 0 and 1).
    return magI;
}

void filter::genFilter(double threshold ,int kSize){
    //gauss spectrum
    cv::Mat gaussMag;
    cv::GaussianBlur(magnitude, gaussMag, cv::Size(kSize,kSize), 0);

    //draw rectangle in gaussMag
    cv::Point p1(230, 205);
    cv::Point p2(285, 305);
    cv::rectangle(gaussMag, p1, p2, cv::Scalar(0, 0, 0), CV_FILLED);

    /*
    for(int row=0; row<gaussMag.rows; row++){
        for(int col=0; col<gaussMag.cols; col++){
            float pixel = gaussMag.at<float>(col,row);
            if(pixel > threshold)
                gaussMag.at<float>(col,row) = 0.0f;
            else
                gaussMag.at<float>(col,row) = 1.0f;
        }
    }

    gaussMag.copyTo(mask);
    */

    double min, max;
    cv::minMaxLoc(gaussMag, &min, &max);
    cv::threshold(gaussMag, mask, threshold, max, cv::THRESH_BINARY_INV);
    cv::normalize(mask, mask, 0, 1, CV_MINMAX);
    imshow("threshold", mask);

    //zum testen
    //mask = cv::imread("C:\\Users\\Marco Pisarczyk\\Desktop\\Dropbox\\FH\\5. Semester\\Bildverarbeitung\\Praktikum\\Images\\filter_test.png");
}

void filter::elWeiseMulti(){
    mult = cv::Mat(magnitude.rows, magnitude.cols, magnitude.type());

    mask = shift(mask);
    cv::Mat planes[] = {cv::Mat::zeros(complexI.size(), CV_32F),
                       cv::Mat::zeros(complexI.size(), CV_32F)};
    cv::Mat kernelSpec;
    planes[0] = mask;   //real
    planes[1] = mask;   //imaginar
    cv::merge(planes, 2, kernelSpec);

    /*
    for(int row=0; row<mask.rows; row++){
        for(int col=0; col<mask.cols; col++){

            //if(mask.at<float>(col,row) == 0.0f)
              //  mult.at<float>(col,row) = 0.0f;
            //else
              //  mult.at<float>(col,row) = magnitude.at<float>(col,row);
            mult.at<float>(col,row) = mask.at<float>(col,row) * magnitude.at<float>(col,row);
        }
    }
    */

    cv::mulSpectrums(complexI, kernelSpec, mult, cv::DFT_ROWS);

    double min, max;
    cv::minMaxLoc(mult, &min, &max);
    //std::cout << min << " " << max << std::endl;

    cv::Mat showMult[2];
    cv::split(mult, showMult);
    showMult[0] = shift(showMult[0]);
    //die mult sieht anders aus als in der pdf, aber es klappt
    //imshow("El.-weise Mult.", showMult[0]);
}

cv::Mat filter::calcInverseTransform(){
    cv::Mat inverseTransform;
    cv::idft(mult, inverseTransform);
    cv::Mat planes[] = {cv::Mat::zeros(complexI.size(), CV_32F),
                       cv::Mat::zeros(complexI.size(), CV_32F)};
    cv::split(inverseTransform, planes);
    cv::magnitude(planes[0],planes[1], inverseTransform);
    cv::normalize(inverseTransform, inverseTransform, 0, 255, cv::NORM_MINMAX);
    inverseTransform.convertTo(inverseTransform, CV_8U);
    return inverseTransform;
}
