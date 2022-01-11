#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //==========================================
    thread_ir = new QThread();
    func_ir = new MyTHread();
    func_ir->moveToThread(thread_ir);

    func_camera = new MyTHread();
    thread_camera = new QThread();
    func_camera->moveToThread(thread_camera);
    //===========================================================
    connect(ui->pb_openIR,&QPushButton::clicked,this,[&](){
        qDebug()<<"打开红外相机";
        thread_ir->start();
        emit signal_openIR(0);
    });
    connect(this,&MainWindow::signal_openIR,func_ir,&MyTHread::videoThread);
    connect(func_ir,&MyTHread::signal_frame,[&](cv::Mat& irImg_){
        irImg = irImg_.clone();
        MyUtils::Mat2QImage(irImg,irQimg);
        qDebug()<<QString("ir红外width:%1;height:%2").arg(irImg.cols).arg(irImg.rows);
        this->update();
    });
    //===========================================================
    connect(ui->pb_openCAM,&QPushButton::clicked,this,[&](){
        qDebug()<<"open camera";
        thread_camera->start();
        emit signal_openCAM(1);
    });
    connect(this,&MainWindow::signal_openCAM,func_camera,&MyTHread::videoThread);
    connect(func_camera,&MyTHread::signal_frame,[&](cv::Mat& Img_){
        cameraImg = Img_.clone();
        MyUtils::Mat2QImage(cameraImg,cameraQimg);
        qDebug()<<QString("ir摄相width:%1;height:%2").arg(cameraImg.cols).arg(cameraImg.rows);
        this->update();
    });


    //===========================================================
    //关闭线程
    connect(this,&MainWindow::destroyed,[&](){
        func_ir->flag_exitCamera=true;
        thread_ir->quit();
        thread_ir->wait();

        func_camera->flag_exitCamera=true;
        thread_camera->quit();
        thread_camera->wait();
        qDebug()<<"关闭线程====";
    });




}

MainWindow::~MainWindow()
{
    delete ui;
    delete thread_ir;
    delete func_ir;
    delete thread_camera;
    delete func_camera;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(ui->ogl_ir);
    //painter.drawImage(QPoint(0, 0),this->frame2QImage);
    painter.drawPixmap(0,0,ui->ogl_ir->width(),ui->ogl_ir->height(),QPixmap::fromImage(this->irQimg));
    painter.end();

    QPainter painter2;
    painter2.begin(ui->ogl_cam);
    painter2.drawPixmap(0,0,ui->ogl_cam->width(),ui->ogl_cam->height(),QPixmap::fromImage(this->cameraQimg));
    painter2.end();
}

void MainWindow::on_pb_openIR_clicked()
{
//    qDebug()<<"打开红外相机";
//    thread_ir->start();
//    emit signal_openIR(0);
}
