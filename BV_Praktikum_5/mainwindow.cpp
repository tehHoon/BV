#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    p = new prak5();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p;
}

void MainWindow::updateGUI(Mat image, Mat processed)
{
    if(!image.empty())
    {
        QImage img(image.data, image.cols, image.rows, static_cast<int>(image.step), QImage::Format_RGB888);
        ui->label_Orig->setPixmap(QPixmap::fromImage(img).scaled(600,600,Qt::KeepAspectRatio));
    }

    if(!processed.empty())
    {
        QImage stuffImg(processed.data, processed.cols, processed.rows, static_cast<int>(processed.step), QImage::Format_RGB888);
        ui->label_Processed->setPixmap(QPixmap::fromImage(stuffImg).scaled(600,600,Qt::KeepAspectRatio));
    }
}

void MainWindow::on_pb_loadImg_clicked()
{
    loadImage(img);
}

void MainWindow::on_pb_loadDistorted_clicked()
{
    loadImage(processed);
}

void MainWindow::loadImage(Mat &dest)
{
    filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.jpg *.png *.bmp)"));
    if(!filename.isEmpty()){
        //read image to img var
        dest = imread(filename.toStdString());

        if(!dest.empty()){
            //BGR->RGB
            cvtColor(dest, dest, CV_BGR2RGB);

            //update GUI to draw img
            updateGUI(img, processed);
        }
    }
}

void MainWindow::on_pb_Features_clicked()
{
    p->findFeatures(img, processed);
}

void MainWindow::on_pb_Trans_clicked()
{
    p->getTransformation(img, processed);
}

void MainWindow::on_pb_SkalRot_clicked()
{

}

void MainWindow::on_pb_OrigImg_clicked()
{
    Mat calc = p->calcOriginal(img, processed);
    //cvtColor(calc, calc, CV_BGR2RGB);
    updateGUI(img, calc);
}

void MainWindow::on_dsb_minHessian_valueChanged(double arg1)
{
    p->setMinHessian(arg1);
}

void MainWindow::on_dsb_Distance_valueChanged(double arg1)
{
    p->setMinDistance(arg1);
}
