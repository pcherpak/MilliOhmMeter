#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QTime>
#include "qcustomplot.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTime time = QTime();

private slots:
    void on_pushButton_Connect_clicked();

    void on_pushButton_Kinetic_Start_clicked();

    void on_action_triggered();
   // void on_action_2_triggered();

    //void get_response(const QString &s);

    void on_pushButton_Send_clicked();

   // void on_pushButton_Receive_clicked();

signals:
    void response(const QString &s);

private:
    Ui::MainWindow *ui;

    QCustomPlot *customPlot_Resistance;
    QCustomPlot *customPlot_Temperature;

    QSerialPort serial;
};

#endif // MAINWINDOW_H
