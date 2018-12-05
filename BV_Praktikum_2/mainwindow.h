#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

#include "histogram.h"

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateGUI(Mat image, Mat hist);

    void on_pbLoad_clicked();
    void on_pbReset_clicked();
    void on_pbStretch_clicked();
    void on_pbLinear_clicked();
    void on_pbGamma_clicked();
    void on_dbGamma_valueChanged(double arg1);
    void on_pbStretch_Gamma_clicked();
    void on_pbStretch_Linear_clicked();

private:
    Ui::MainWindow *ui;
    Mat img;            //original image
    QString filename;
    histogram *h;
    double gamma = 1.0;

    Mat getHistogramm(Mat src);
    void enableButtons();
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
