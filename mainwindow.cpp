#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    on_action_triggered();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Connect_clicked()
{
    if (ui->pushButton_Connect->text() == "Connect") {

        serial.setPortName(ui->comboBox_Port_Name->currentText());

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

    on_pushButton_Receive_clicked();
}
