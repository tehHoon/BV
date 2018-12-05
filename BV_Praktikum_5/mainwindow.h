#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <prak5.h>

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
    void updateGUI(Mat src, Mat processed);
    void loadImage(Mat &dest);

private slots:
    void on_pb_loadImg_clicked();
    void on_pb_Features_clicked();
    void on_pb_Trans_clicked();
    void on_pb_SkalRot_clicked();
    void on_pb_OrigImg_clicked();
    void on_pb_loadDistorted_clicked();

    void on_dsb_minHessian_valueChanged(double arg1);

    void on_dsb_Distance_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    prak5 *p;

    QString filename;
    Mat img, processed;

};

#endif // MAINWINDOW_H
