#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_action_triggered();

    {
        ui->lcdNumber_Time->display(QString::number(time.second()));
        ui->lineEdit_Command->setText(QString::number(time.second()));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Connect_clicked()
{
    if (ui->pushButton_Connect->text() == "Connect") {
        QString ActivationCommand = "[+]";
        serial.write(ActivationCommand.toLocal8Bit());
        serial.setPortName(ui->comboBox_Port_Name->currentText());
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);

        ui->pushButton_Connect->setText("Disconnect");
    } else {
        serial.close();

        ui->pushButton_Connect->setText("Connect");

    }
}

void MainWindow::on_action_triggered()
{
    ui->comboBox_Port_Name->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBox_Port_Name->addItem(info.portName());
    }

    ui->lineEdit_Response->setText("Clicked!");

    return;
}

void MainWindow::on_pushButton_Send_clicked()
{
    QString command = ui->lineEdit_Command->text();

    serial.write(command.toLocal8Bit());

}

void MainWindow::on_pushButton_Kinetic_Start_clicked()
{
    serial.write("[?D]");
    ui->Current_Resistance_Display->display(QTextCodec::codecForMib(1015)->toUnicode(serial.readLine()));
    serial.write("[?R]");
    ui->Current_Range_Display->display(QTextCodec::codecForMib(1015)->toUnicode(serial.readLine()));
    time.start();

    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
    x[i] = i/50.0 - 1; // x goes from -1 to 1
    y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->customPlot_Temperature->addGraph();
    ui->customPlot_Temperature->graph(0)->setData(x, y);
    ui->customPlot_Temperature->xAxis->setLabel("Time (s)");
    ui->customPlot_Temperature->yAxis->setLabel("Temperature (grad)");
    ui->customPlot_Temperature->replot();

    ui->customPlot_Resistance->addGraph();
    ui->customPlot_Resistance->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot_Resistance->xAxis->setLabel("Time (s)");
    ui->customPlot_Resistance->yAxis->setLabel("Resistance (Ohm)");
    // set axes ranges, so we see all data:
    ui->customPlot_Resistance->xAxis->setRange(-1, 1);
    ui->customPlot_Resistance->yAxis->setRange(0, 1);
    ui->customPlot_Resistance->replot();
}
