#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QTime>
#include <QVector>
#include "qcustomplot.h"
#include "help.h"
#include "milliohmmeter.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void Hide_Show_Stuff(bool tf);

    void on_pushButton_RC_enable_clicked();

    void on_pushButton_Connect_clicked();

    void on_pushButton_Range_UP_clicked();

    void on_pushButton_Range_DOWN_clicked();

    void on_pushButton_20mOhm_clicked();

    void on_pushButton_Auto_Range_clicked();

    void on_pushButton_Kinetic_Start_clicked();

    void on_actionHelp_triggered();

    void on_pushButton_Send_clicked();

    void on_pushButton_HOLD_clicked();

    void on_pushButton_RATE_clicked();

    void on_pushButton_NULL_Correction_clicked();

    void on_pushButton_LOCAL_clicked();

    void on_pushButton_COMPARE_clicked();

    void on_pushButton_HIGH_clicked();

    void on_pushButton_LOW_clicked();


    void on_pushButton_Bleeper_clicked();

    void Read_Settings();
    void on_customPlot_Resistance_Moved(QMouseEvent *event);
    void get_response(QString);

    void on_action_COM_Port_Update_triggered();

    void on_pushButton_Choose_File_for_Export_clicked();

    void on_pushButton_import_clicked();

signals:
    void response(const QString &s);
    //void mouseMove(QMouseEvent*);

    //help
    void infoSignal(const QString,const QString);

private:
    Ui::MainWindow *ui;
    Help *help;
    MilliOhmMeter *milliOhmMeter;

    QCustomPlot *customPlot_Resistance;
    QTime *time;

    QVector<double> x;
    QVector<double> y;

    bool BOOOL;
    };

#endif // MAINWINDOW_H
