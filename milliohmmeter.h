#ifndef MILLIOHMMETER_H
#define MILLIOHMMETER_H

#include <QObject>
#include <QTime>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class MilliOhmMeter : public QObject
{
    Q_OBJECT
public:
    MilliOhmMeter();
    bool connectSerial(const QString&);
    void disconnectSerial();
    QSerialPort::SerialPortError serialError();

    QString receive();
    void send(const QString&);

    void activation();
    void deactivation();

    void lowLimit(QString);
    void highLimit(QString);
    QString lowLimit();
    QString highLimit();

    bool autoRange();
    void autoRange(bool);

    bool hold();
    void hold(bool);

    bool nullCollection();
    void nullCollection(bool);

    bool beeper();
    void beeper(bool);

    bool rate();
    void rate(bool);

    void range20mOhm();

    void local();

    void downRange();
    void upRange();

    double value(); //Ohms

    void compare(bool);
    bool compare();
    QString compareResult();

    QString softwareVersion();
    QString serialNumber();
signals:
    void error(QString);
private:
    QSerialPort serial;

    int firstWaitTime = 500;
    int additionalWaitTime = 150;

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


#endif // MILLIOHMMETER_H
