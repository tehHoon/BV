#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f = new filter();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateGUI(Mat image, Mat stuff){
    QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    ui->labelImg->setPixmap(QPixmap::fromImage(img).scaled(600,600,Qt::KeepAspectRatio));

    if(!stuff.empty()){
        QImage stuffImg(stuff.data, stuff.cols, stuff.rows, stuff.step, QImage::Format_Indexed8);
        ui->labelStuff->setPixmap(QPixmap::fromImage(stuffImg).scaled(600,600,Qt::KeepAspectRatio));
    }
}

void MainWindow::on_pbLoad_clicked() {
    filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.jpg *.png *.bmp)"));

    //read image to img var
    img = imread(filename.toStdString());

    if(!img.empty()){
        //update GUI to draw img
        updateGUI(img, Mat());
    }
}

void MainWindow::on_pbLogShiftedSpectrum_clicked(){
    if(!img.empty()){
        f->fourier(img);
        updateGUI(img, Mat());
        lssAvailable = true;
    }
}

void MainWindow::on_pbGenerateFilter_clicked() {
    if(!img.empty() && lssAvailable){
        f->genFilter(threshold, kSize);
        filterGen = true;
    }
}

void MainWindow::on_sbKSize_valueChanged(int arg1) {
    if(!img.empty() && lssAvailable){
        kSize = arg1;
        on_pbGenerateFilter_clicked();
    }
}

void MainWindow::on_dsbThreshold_valueChanged(double arg1) {
    if(!img.empty() && lssAvailable){
        threshold = arg1;
        on_pbGenerateFilter_clicked();
    }
}

void MainWindow::on_pbElMulti_clicked() {
    if(!img.empty() && filterGen){
        f->elWeiseMulti();
        multiFilter = true;
    }
}

void MainWindow::on_pbRuecktransformieren_clicked() {
    if(!img.empty() && multiFilter){
        Mat rueck = f->calcInverseTransform();
        updateGUI(img, rueck);
    }
}
