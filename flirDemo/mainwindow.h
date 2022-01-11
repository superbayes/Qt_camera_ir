#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mythread.h"
#include "myutils.h"
#include <QThread>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    QThread* thread_ir;
    QThread* thread_camera;
    MyTHread* func_ir;
    MyTHread* func_camera;

public:
    cv::Mat irImg;
    QImage irQimg;
    cv::Mat cameraImg;
    QImage cameraQimg;
public:
    void paintEvent(QPaintEvent *e);
signals:
    void signal_openIR(int deviceid);
    void signal_openCAM(int deviceid);
private slots:
    void on_pb_openIR_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
