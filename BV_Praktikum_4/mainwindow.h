#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include "filter.h"

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateGUI(Mat image, Mat stuff);

private slots:
    void on_pbLoad_clicked();
    void on_pbLogShiftedSpectrum_clicked();

    void on_pbGenerateFilter_clicked();

    void on_sbKSize_valueChanged(int arg1);

    void on_dsbThreshold_valueChanged(double arg1);

    void on_pbElMulti_clicked();

    void on_pbRuecktransformieren_clicked();

private:
    Ui::MainWindow *ui;
    Mat img;            //original image
    QString filename;
    filter *f;

    bool lssAvailable = false;
    bool filterGen = false;
    bool multiFilter = false;
    double threshold = 0.5;
    int kSize = 1;
};

#endif // MAINWINDOW_H
