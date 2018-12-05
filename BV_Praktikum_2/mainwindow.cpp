#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    h = new histogram();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateGUI(Mat image, Mat hist){
    //display image on label via setPixmap
    ui->labelImage->setPixmap(QPixmap::fromImage(QImage(image.data, image.cols,
                                                        image.rows, image.step, QImage::Format_Indexed8)));

    ui->labelHistogramm->setPixmap(QPixmap::fromImage(QImage(hist.data, hist.cols,
                                                             hist.rows, hist.step, QImage::Format_RGB888)));

}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    //stuff if window resize event is called
}

void MainWindow::enableButtons(){
    ui->pbStretch->setEnabled(true);
    ui->pbStretch_Gamma->setEnabled(true);
    ui->pbStretch_Linear->setEnabled(true);
    ui->pbGamma->setEnabled(true);
    ui->pbLinear->setEnabled(true);
    ui->pbReset->setEnabled(true);
    ui->dbGamma->setEnabled(true);
}

Mat MainWindow::getHistogramm(Mat src) {
    return h->createHistogram(src);
}

void MainWindow::on_pbLoad_clicked() {
    filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.jpg *.png *.bmp)"));

    //read image to img var
    img = imread(filename.toStdString());

    if(!img.empty()){
        //enable all buttons
        enableButtons();

        //if image not grayscale, convert to grayscale
        if(img.channels()>1){
            cvtColor(img, img, CV_BGR2GRAY);
        }

        Mat hist = getHistogramm(img);

        //update GUI to draw img & histogramm
        updateGUI(img, hist);
    }
}

void MainWindow::on_pbReset_clicked() {
    Mat hist = getHistogramm(img);
    updateGUI(img, hist);
}

void MainWindow::on_pbStretch_clicked() {
   Mat stretched = h->stretchHistogram(img, 1.0);
   Mat stretchedHist = getHistogramm(stretched);
   updateGUI(stretched, stretchedHist);
}

void MainWindow::on_pbLinear_clicked() {
    Mat linear = h->linearHistogram(img);
    Mat linearHist = getHistogramm(linear);
    updateGUI(linear, linearHist);
}

void MainWindow::on_pbGamma_clicked() {
    Mat stretched = h->stretchHistogram(img,gamma);
    Mat stretchedHist = getHistogramm(stretched);
    updateGUI(stretched, stretchedHist);
}

void MainWindow::on_dbGamma_valueChanged(double arg1) {
    this->gamma = arg1;
}

void MainWindow::on_pbStretch_Gamma_clicked() {
    Mat stretched = h->stretchHistogram(img, 1.0);
    Mat gammaStretched = h->stretchHistogram(stretched, gamma);
    Mat gammaStrHisto = getHistogramm(gammaStretched);
    updateGUI(gammaStretched, gammaStrHisto);
}

void MainWindow::on_pbStretch_Linear_clicked() {
    Mat stretched = h->stretchHistogram(img, 1.0);
    Mat linear = h->linearHistogram(stretched);
    Mat strLinHist = getHistogramm(linear);
    updateGUI(linear, strLinHist);
}
