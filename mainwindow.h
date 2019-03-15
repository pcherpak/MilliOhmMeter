#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QTime>
#include "qcustomplot.h"
#include "help.h"

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

    void Receive();

    void Hide_Show_Stuff(bool tf);

    //void Set_Data();

    //void Export_Data(double x, double y);

    void on_pushButton_RC_enable_clicked();

    void on_pushButton_Connect_clicked();

    void on_pushButton_Range_UP_clicked();

    void on_pushButton_Range_DOWN_clicked();

    void on_pushButton_20mOhm_clicked();

    void on_pushButton_Auto_Range_clicked();

    void on_pushButton_Kinetic_Start_clicked();

    void on_action_triggered();
   // void on_action_2_triggered();
    void on_actionHelp_triggered();

   // void process_stop_signal(const QString &s);

    void get_response(const QString &s);

    void on_pushButton_Send_clicked();

    //void timerStart();

   // void on_pushButton_Receive_clicked();

    void on_pushButton_HOLD_clicked();

    void on_pushButton_RATE_clicked();

    void on_pushButton_NULL_Correction_clicked();

    void on_pushButton_LOCAL_clicked();

    void on_pushButton_COMPARE_clicked();

    //void on_pushButton_HIGH_LIMIT_clicked();

    //void on_pushButton_Switch_function_set_clicked();

   // void on_pushButton_ARROW_RIGHT_clicked(const QString &s);

    void on_pushButton_HIGH_clicked();

    void on_pushButton_LOW_clicked();

    //void on_pushButton_STOP_clicked();

    void on_pushButton_Bleeper_clicked();

    void Read_Settings();
    void on_customPlot_Resistance_Moved(QMouseEvent *event);

signals:

    //void stop_signal(const QString &s);

    //void HighLimitButton(const QString &s);
    void response(const QString &s);

    //help
    void infoSignal(const QString,const QString);

private:
    Ui::MainWindow *ui;
    Help *help;

    QCustomPlot *customPlot_Resistance;
    QCustomPlot *customPlot_Temperature;

    QTime *time;

    QSerialPort serial;

    QVector<double> x;
    QVector<double> y;

    bool BOOOL;

    QString ActivationCommand = "[+]";
    QString DeactivationCommand = "[-]";
    QString R1Command = "[R1]";
    QString R2Command = "[R2]";
    QString R3Command = "[R3]";
    QString R4Command = "[R4]";
    QString R5Command = "[R5]";
    QString R6Command = "[R6]";
    QString R7Command = "[R7]";
    QString R8Command = "[R8]";
    QString R9Command = "[R9]";
    QString AutoRangeCommand = "[R01]";
    QString G1Command = "[G1]";
    QString G0Command = "[G0]";
    QString F1Command = "[F1]";
    QString F0Command = "[F0]";
    QString I1Command = "[I1]";
    QString I0Command = "[I0]";
    QString C1Command = "[C1]";
    QString C0Command = "[C0]";
    QString B1Command = "[B1]";
    QString B0Command = "[B0]";
    //QString HCommand = "[H=]";
    //QString LCommand = "[L=]";
    QString M1Command = "[M1]"; //High
    QString M2Command = "[M2]"; //Go
    QString M3Command = "[M3]"; //Low

    QString SoftwareVersionCommand = "[?VER]";
    QString SerialNumberCommand = "[?MOD]";
    QString RangeQuery = "[?R]";
    QString ValueQuery = "[?D]";
    QString HoldQuery = "[?G]";
    QString RateQuery = "[?F]";
    QString NullCorrectionQuery = "[?I]";
    QString CompareStatusQuery = "[?C]";
    QString BeeperQuery = "[?B]";
    QString HighLimitQuery = "[?H]";
    QString LowLimitQuery = "[?L]";
    QString CompareResultQuery = "[?M]";
    };

#endif // MAINWINDOW_H
