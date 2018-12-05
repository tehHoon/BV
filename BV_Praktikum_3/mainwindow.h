#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <vector>
#include <opencv2/opencv.hpp>

#include "filter.h"

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
    void updateGUI(Mat image, Mat fImage);

    void on_pbLoadImage_clicked();

    void on_pbH1_clicked();
    void on_pbH2_clicked();
    void on_pbH3_clicked();

    void on_sbFilter_valueChanged(int arg1);
    void on_pbNormMittel_clicked();
    void on_pbGauss_clicked();

    void on_pbXAbl_clicked();
    void on_pbYAbl_clicked();
    void on_pbLaplace_clicked();
    void on_pbCannyEdge_clicked();
    void on_pbSobelX_clicked();
    void on_pbSobelY_clicked();
    void on_pbGrad_clicked();

    void on_dsbCannyMin_valueChanged(double arg1);

    void on_dspCannyMax_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    Mat img;            //original image
    QString filename;
    filter *f;

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
