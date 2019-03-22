#include "milliohmmeter.h"

MilliOhmMeter::MilliOhmMeter()
{

}

bool MilliOhmMeter::connectSerial(const QString &s)
{
    serial.setPortName(s);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    return serial.open(QIODevice::ReadWrite);
}

void MilliOhmMeter::disconnectSerial()
{
    serial.close();
}

QSerialPort::SerialPortError MilliOhmMeter::serialError()
{
    return serial.error();
}

void MilliOhmMeter::send(const QString &s)
{
    serial.write(s.toLocal8Bit());
}

void MilliOhmMeter::activation()
{
    send(ActivationCommand);
}

void MilliOhmMeter::deactivation()
{
    send(DeactivationCommand);
}

void MilliOhmMeter::lowLimit(QString s)
{
    if(s.length() > 5)
    {
        emit error("Error. The format for Low Limit is of following: XXXXX");
        return;
    }
    while(s.length() != 5)
    {
        s.prepend("0");
    }
    send("[L="+s+"]");
}

void MilliOhmMeter::highLimit(QString s)
{
    if(s.length() > 5)
    {
        emit error("Error. The format for High Limit is of following: XXXXX");
        return;
    }
    while(s.length() != 5)
    {
        s.prepend("0");
    }
    send("[H="+s+"]");
}

QString MilliOhmMeter::lowLimit()
{
    send(LowLimitQuery);
    QString Low = receive();
    Low.chop(1);
    Low.remove(0,3);
    return Low;
}

QString MilliOhmMeter::highLimit()
{
    send(HighLimitQuery);
    QString High = receive();
    High.chop(1);
    High.remove(0,3);
    return High;
}

bool MilliOhmMeter::autoRange()
{
    send(RangeQuery);
    if(receive().contains("R01"))
        return true;
    else {
        return false;
    }
}

void MilliOhmMeter::autoRange(bool)
{
    send(AutoRangeCommand);
}

bool MilliOhmMeter::hold()
{
    send(HoldQuery);
    if(receive().contains("G1"))
        return true;
    else {
        return false;
    }
}

void MilliOhmMeter::hold(bool bl)
{
    if(bl)
        send(G0Command);
    else
        send(G1Command);
}

bool MilliOhmMeter::nullCollection()
{
    send(NullCorrectionQuery);
    if(receive().contains("I1"))
        return true;
    else {
        return false;
    }
}

void MilliOhmMeter::nullCollection(bool bl)
{
    if(bl)
        send(I1Command);
    else
        send(I0Command);
}

bool MilliOhmMeter::beeper()
{
    send(BeeperQuery);
    if(receive().contains("B1"))
        return true;
    else {
        return false;
    }
}

void MilliOhmMeter::beeper(bool bl)
{
    if(bl)
        send(B1Command);
    else
        send(B0Command);
}

bool MilliOhmMeter::rate()
{
    send(RateQuery);
    if(receive().contains("F1"))
        return true;
    else {
        return false;
    }
}

void MilliOhmMeter::rate(bool bl)
{
    if(bl)
        send(F1Command);
    else
        send(F0Command);
}

void MilliOhmMeter::range20mOhm()
{
    send(R1Command);
}

void MilliOhmMeter::local()
{
    send(DeactivationCommand);
}

void MilliOhmMeter::downRange()
{
    send(RangeQuery);
    QString range = receive();
    if(range.contains("[R9]")) {
        send(R8Command);
        return;
    }
    if(range.contains("[R8]")) {
        send(R7Command);
        return;
    }
    if(range.contains("[R7]")) {
        send(R6Command);
        return;
    }
    if(range.contains("[R6]")) {
        send(R5Command);
        return;
    }
    if(range.contains("[R5]")) {
        send(R4Command);
        return;
    }
    if(range.contains("[R4]")) {
        send(R3Command);
        return;
    }
    if(range.contains("[R3]")) {
        send(R2Command);
        return;
    }
    if(range.contains("[R2]")) {
        send(R1Command);
        return;
    }
}

void MilliOhmMeter::upRange()
{
    send(RangeQuery);
    QString range = receive();
    if(range.contains("[R8]")) {
        send(R9Command);
        return;
    }
    if(range.contains("[R7]")) {
        send(R8Command);
        return;
    }
    if(range.contains("[R6]")) {
        send(R7Command);
        return;
    }
    if(range.contains("[R5]")) {
        send(R6Command);
        return;
    }
    if(range.contains("[R4]")) {
        send(R5Command);
        return;
    }
    if(range.contains("[R3]")) {
        send(R4Command);
        return;
    }
    if(range.contains("[R2]")) {
        send(R3Command);
        return;
    }
    if(range.contains("[R1]")) {
        send(R2Command);
        return;
    }
}

double MilliOhmMeter::value()
{
    send(ValueQuery);
    QString CurrentResistance = receive();

    if(CurrentResistance.contains("mOhm")) {
        CurrentResistance.chop(5);
        CurrentResistance.remove(0,3);
        double currentResistance = CurrentResistance.toDouble();
        return currentResistance = currentResistance/1000;
    }
    if(CurrentResistance.contains("KOhm")) {
        CurrentResistance.chop(5);
        CurrentResistance.remove(0,3);
        double currentResistance = CurrentResistance.toDouble();
        return currentResistance = currentResistance * 1000;
    }
    if(CurrentResistance.contains("Ohm")) {
        CurrentResistance.chop(4);
        CurrentResistance.remove(0,3);
        double currentResistance = CurrentResistance.toDouble();
        return currentResistance = currentResistance/1000;
    }
    if(CurrentResistance.contains("MOhm")) {
        CurrentResistance.chop(5);
        CurrentResistance.remove(0,3);
        double currentResistance = CurrentResistance.toDouble();
        return currentResistance = currentResistance * 1000000;
    }
    else
        return 0;
}

void MilliOhmMeter::compare(bool bl)
{
    if(bl)
        send(C1Command);
    else
        send(C0Command);
}

bool MilliOhmMeter::compare()
{
    send(CompareStatusQuery);
    QString s = receive();
    if(s.contains("[C0]"))
        return false;
    else
        return true;
}

QString MilliOhmMeter::compareResult()
{
    send(CompareResultQuery);
    QString s = receive();
    if(s.contains("[M1]"))
        return "Current Resistance higher than High Limit";
    if(s.contains("[M2]"))
        return "Current Resistance is a Go";
    if(s.contains("[M3]"))
        return "Current Resistance lower than Low Limit";
    else
        return "Something has gone wrong";
}

QString MilliOhmMeter::softwareVersion()
{
    send(SoftwareVersionCommand);
    return receive();
}

QString MilliOhmMeter::serialNumber()
{
    send(SerialNumberCommand);
    return receive();
}

QString MilliOhmMeter::receive()
{
    QString temp = "";
    QByteArray array;
    if (serial.waitForReadyRead(firstWaitTime)) {
        array = serial.readAll();
        temp = QString(array);

        while (serial.waitForReadyRead(additionalWaitTime)) {
            array = serial.readAll();
            temp += QString(array);
        }
    }
    return temp.trimmed();
}
