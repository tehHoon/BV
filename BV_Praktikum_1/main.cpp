#include <QApplication>
#include <iostream>
#include <QFile>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat loadFromQrc(QString qrc, int flag = IMREAD_COLOR) {
    QFile file(qrc);
    Mat m;
    if(file.open(QIODevice::ReadOnly)) {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        m = imdecode(buf, flag);
    }

    return m;
}

void stretch(double &g, double wMin, double wMax, double gMin, double gMax){
    g = (wMax-wMin)*( (g-gMin)/(gMax-gMin) ) + wMin;
}

void getChannels(vector<Mat> &output, Mat src){
    output.clear();
    split(src, output);
}

void GenerateAnaglyph(Mat &output, Mat src){
    Mat links = src;
    Mat rechts = loadFromQrc(":/img/right.jpg");

    vector<Mat> linksChannel;
    vector<Mat> rechtsChannel;
    split(links, linksChannel);
    split(rechts, rechtsChannel);

    Mat temp[3];
    temp[0] = rechtsChannel.at(0);
    temp[1] = rechtsChannel.at(1);
    temp[2] = linksChannel.at(2);

    merge(temp, 3, output);
}

void BGR2CMY(Mat &output, Mat src){
    Mat rgb;
    cvtColor(src, rgb, COLOR_BGR2RGB);

    vector<Mat> channels;
    vector<Mat> rgbChannels;
    split(rgb, rgbChannels);

    channels.push_back(255-rgbChannels.at(2));
    channels.push_back(255-rgbChannels.at(1));
    channels.push_back(255-rgbChannels.at(0));
    merge(channels, output);
}

Mat BGR2YIQ(Mat src){
    Mat output;
    //zu CV_64FC3 für double 3channel statt CV_32FC3 float
    src.convertTo(output, CV_64FC3);

    double r, g, b, y, i, q;

    for(int m=0; m<src.rows; m++){
        for(int n=0; n<src.cols; n++){
            //rgb pro pixel finden und normalisieren [0,1]
            r = src.at<Vec3b>(Point(n,m))[2]/255.0;
            g = src.at<Vec3b>(Point(n,m))[1]/255.0;
            b = src.at<Vec3b>(Point(n,m))[0]/255.0;

            y = 0.299*r + 0.587*g + 0.114*b;
            i = 0.596*r - 0.275*g - 0.321*b;
            q = 0.212*r - 0.523*g + 0.311*b;

            // wieder zurück zu [0,1]
            // y ist durch rgb [0,1] auch in [0,1] range
            stretch(i, 0.0, 1.0, -0.596, 0.596);
            stretch(q, 0.0, 1.0, -0.523, 0.523);

            //yiq pro pixel einsetzen
            output.at<Vec3d>(Point(n,m))[0] = y;
            output.at<Vec3d>(Point(n,m))[1] = i;
            output.at<Vec3d>(Point(n,m))[2] = q;
        }
    }
    return output;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //load image from Qrc
    Mat image = loadFromQrc(":/img/left.jpg");

    // Check for invalid input
    if( image.empty() ){
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    //create window named P1
    namedWindow("P1", WINDOW_NORMAL);

    //Anaglyph
    Mat anaglyph;
    GenerateAnaglyph(anaglyph, image);

    //BGR->HSV
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    //BGR->CMY
    Mat cmy;
    BGR2CMY(cmy, image);

    //BGR->YIQ (NTSC)
    //Mat yiq(image.rows, image.cols, CV_64FC3);
    Mat yiq = BGR2YIQ(image);

    //BGR->Lab
    Mat Lab;
    cvtColor(image, Lab, COLOR_BGR2Lab);

    //init picture
    imshow("P1", image);

    //channel mat
    vector<Mat> channel;
    getChannels(channel, image);

    while(true){
        int key = waitKey();
        switch(key){
                //Orig img  1
            case 49: imshow("P1", image); getChannels(channel, image); break;
                //Anaglyph  2
            case 50: imshow("P1", anaglyph); getChannels(channel, anaglyph); break;
                //HSV       3
            case 51: imshow("P1", hsv); getChannels(channel, hsv); break;
                //CMY       4
            case 52: imshow("P1", cmy); getChannels(channel, cmy); break;
                //YIQ (NTSC) 5
            case 53: imshow("P1", yiq); getChannels(channel, yiq); break;
                //Lab       6
            case 54: imshow("P1", Lab); getChannels(channel, Lab); break;
                //Channel 1 q
            case 113:
            case 81: imshow("P1", channel.at(0)); break;
                //Channel 2 w
            case 119:
            case 87: imshow("P1", channel.at(1)); break;
                //Channel 3 e
            case 101:
            case 69: imshow("P1", channel.at(2)); break;
                //ESC
            case 27: destroyAllWindows(); return a.exec();
        }
    }
}
