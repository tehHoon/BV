#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    p = new prak6();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p;
}

void MainWindow::updateGUI(Mat image)
{
    if(!image.empty())
    {
        QImage img(image.data, image.cols, image.rows, static_cast<int>(image.step), QImage::Format_RGB888);
        ui->labelImg->setPixmap(QPixmap::fromImage(img).scaled(600,600,Qt::KeepAspectRatio));
    }
}

void MainWindow::on_pb_load_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.jpg *.png *.bmp)"));
    if(!filename.isEmpty()){
        //read image to img var
        img = imread(filename.toStdString());

        if(!img.empty()){
            //update GUI to draw img
            updateGUI(img);
        }
    }
}

void MainWindow::on_pbHintergrund_clicked()
{
    p->getBackgroundImage(img);
}

void MainWindow::on_pb_Binaer_clicked()
{
    p->binaryImage();
}

void MainWindow::on_dsb_threshMax_valueChanged(double arg1)
{
    p->setBThreshMax(arg1);
    on_pb_Binaer_clicked();
}

void MainWindow::on_dsb_threshMin_valueChanged(double arg1)
{
    p->setBThreshMin(arg1);
    on_pb_Binaer_clicked();
}

void MainWindow::on_pbOpening_clicked()
{
    p->Opening();
}

void MainWindow::on_pbClosing_clicked()
{
    p->Closing();
}

void MainWindow::on_pbUeberlagern_clicked()
{
    p->Overlap();
}

void MainWindow::on_pb_AIO_clicked()
{
    p->printAllResults(img);
}
