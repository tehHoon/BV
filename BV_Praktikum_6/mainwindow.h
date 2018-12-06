#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <prak6.h>

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
    void updateGUI(Mat src);

private slots:
    void on_pb_load_clicked();

    void on_pbHintergrund_clicked();

    void on_pb_Binaer_clicked();
    void on_dsb_threshMax_valueChanged(double arg1);
    void on_dsb_threshMin_valueChanged(double arg1);

    void on_pbOpening_clicked();

    void on_pbClosing_clicked();

    void on_pbUeberlagern_clicked();

    void on_pb_AIO_clicked();

private:
    Ui::MainWindow *ui;
    prak6 *p;
    Mat img;
    QString filename;

};

#endif // MAINWINDOW_H
