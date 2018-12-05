#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f = new filter();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateGUI(Mat image, Mat fImage){
    if(fImage.empty()){
        fImage = image;
    }

    image.convertTo(image, CV_8UC3);

    QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_Indexed8);
    QImage fImg(fImage.data, fImage.cols, fImage.rows, fImage.step, QImage::Format_Indexed8);

    //display image on label via setPixmap
    ui->labelImage->setPixmap(QPixmap::fromImage(img).scaled(600,600,Qt::KeepAspectRatio));
    ui->labelFilteredImage->setPixmap(QPixmap::fromImage(fImg).scaled(600,600,Qt::KeepAspectRatio));
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    //stuff if window resize event is called
}

void MainWindow::on_pbLoadImage_clicked() {
    filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.jpg *.png *.bmp)"));

    //read image to img var
    img = imread(filename.toStdString());

    if(!img.empty()){
        //if image not grayscale, convert to grayscale
        if(img.channels()>1){
            cvtColor(img, img, CV_BGR2GRAY);
        }

        //update GUI to draw img
        updateGUI(img, Mat());
        ui->labelOrig->setText("Orignalbild");
        ui->labelFiltername->setText("Orignalbild");
    }
}

/* Aufgabe 3.2 Teil 1 */
void MainWindow::on_pbH1_clicked() {
    if(!img.empty()){
        Mat filter = (Mat_<int>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
        Mat h = f->falten(filter, img);
        updateGUI(img, h);
        ui->labelFiltername->setText("Faltung mit h1");
    }
}

void MainWindow::on_pbH2_clicked() {
    if(!img.empty()){
        Mat filter = (Mat_<double>(3, 3) << 1, 2, 1, 2, 4, 2, 1, 2, 1)/16;
        Mat h = f->falten(filter, img);
        updateGUI(img, h);
        ui->labelFiltername->setText("Faltung mit h2");
    }
}

void MainWindow::on_pbH3_clicked() {
    if(!img.empty()){
        Mat filter = (Mat_<int>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
        Mat h = f->falten(filter, img);
        updateGUI(img, h);
        ui->labelFiltername->setText("Faltung mit h3");
    }
}

/* Aufgabe 3.3 Teil 2 a */
void MainWindow::on_sbFilter_valueChanged(int arg1) {
    f->setKernelSize(arg1);
}

void MainWindow::on_pbNormMittel_clicked() {
    if(!img.empty()){
        Mat norm = f->normMittelFilter(img);
        updateGUI(img, norm);
        ui->labelFiltername->setText(QString("Normierter Mittelwertsfilter, Filtergröße " + QString::number(ui->sbFilter->value())));
    }
}

void MainWindow::on_pbGauss_clicked() {
    if(!img.empty()){
        Mat gauss = f->gauss(img);
        updateGUI(img, gauss);

        double sigma = 0.3 * ((ui->sbFilter->value()-1)*0.5 - 1) + 0.8;
        ui->labelFiltername->setText(QString("Gaußfilter mit Filtergröße " + QString::number(ui->sbFilter->value()) + ", Sigma von " + QString::number(sigma)));
    }
}

/* Aufgabe 3.3 Teil 2 b */
void MainWindow::on_pbXAbl_clicked() {
    if(!img.empty()){
        Mat xAbl = f->xDeriv(img);
        updateGUI(img, xAbl);
        ui->labelFiltername->setText(QString("1. Ableitung in x-Richtung, Rückwärtsgradient"));
    }
}

void MainWindow::on_pbYAbl_clicked() {
    if(!img.empty()){
        Mat yAbl = f->yDeriv(img);
        updateGUI(img, yAbl);
        ui->labelFiltername->setText(QString("1. Ableitung in y-Richtung, Vorwärtsgradient"));
    }
}

void MainWindow::on_pbLaplace_clicked() {
    if(!img.empty()){
        Mat laplace = f->laPlace(img);
        updateGUI(img, laplace);
        ui->labelFiltername->setText(QString("Laplace Filter"));
    }
}

void MainWindow::on_pbSobelX_clicked() {
    if(!img.empty()){
        Mat sobX = f->sobelX(img);
        updateGUI(img, sobX);
        ui->labelFiltername->setText(QString("Sobel Filter in x-Richtung"));
    }
}

void MainWindow::on_pbSobelY_clicked() {
    if(!img.empty()){
        Mat sobY = f->sobelY(img);
        updateGUI(img, sobY);
        ui->labelFiltername->setText(QString("Sobel Filter in y-Richtung"));
    }
}

void MainWindow::on_pbGrad_clicked() {
    if(!img.empty()){
        Mat grad = f->sobel(img);
        updateGUI(img, grad);
        ui->labelFiltername->setText(QString("Gradientenbetrag nach Sobel"));
    }
}

void MainWindow::on_pbCannyEdge_clicked() {
    if(!img.empty()){
        Mat canny = f->canny(img);
        updateGUI(img, canny);
        ui->labelFiltername->setText(QString("Canny-Edge Detector"));
    }
}

void MainWindow::on_dsbCannyMin_valueChanged(double arg1) {
    f->setCannyMin(arg1);
    on_pbCannyEdge_clicked();
}

void MainWindow::on_dspCannyMax_valueChanged(double arg1) {
    f->setCannyMax(arg1);
    on_pbCannyEdge_clicked();
}
