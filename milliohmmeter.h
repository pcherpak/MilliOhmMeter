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
    void disconnectSerial() const;
    QSerialPort::SerialPortError serialError() const;

    QString receive() const;
    void send(const QString&) const;

    void activation() const;
    void deactivation() const;

    void lowLimit(QString) const;
    void highLimit(QString) const;
    QString lowLimit() const;
    QString highLimit() const;

    bool autoRange() const;
    void autoRange(bool) const;

    bool hold() const;
    void hold(bool) const;

    bool nullCollection() const;
    void nullCollection(bool) const;

    bool beeper() const;
    void beeper(bool) const;

    bool rate() const;
    void rate(bool) const;

    void range20mOhm() const;

    void local() const;

    void downRange() const;
    void upRange() const;

    double value() const; //Ohms

    void compare(bool) const;
    bool compare() const;
    QString compareResult() const;

    QString softwareVersion() const;
    QString serialNumber() const;
signals:
    void error(QString) const;
private:
    QSerialPort *serial;

    const int firstWaitTime = 500;
    const int additionalWaitTime = 150;

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
