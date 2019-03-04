#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    on_action_triggered();

    Hide_Show_Stuff(false);

    ui->lineEdit_Response->setEnabled(false);

    //ui->pushButton_Send->setEnabled(false);

    ui->pushButton_Export->setEnabled(false);

    //ui->checkBox_Remote_Control->setVisible(false);
    ui->checkBox_Remote_Control->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBox_Remote_Control->setFocusPolicy(Qt::NoFocus);
    ui->checkBox_AUTO->setEnabled(false);
    ui->checkBox_NULL->setEnabled(false);
    ui->checkBox_HOLD->setEnabled(false);
    ui->checkBox_Bleeper->setEnabled(false);
    
    
    connect(this, SIGNAL(response(QString)), this, SLOT(get_response(QString)));
   // connect(this, SIGNAL(HighLimitButton(QString)), this, SLOT(on_pushButton_ARROW_RIGHT_clicked()));

    time = new QTime();
    connect(ui->pushButton_Kinetic_Start, SIGNAL(released()), this, SLOT(timerStart()));
    //connect(boy+listener, SIGNAL(reciebve), this, SLOT(setData()))

    help = new(Help); //Help Window Created
    connect(this,&MainWindow::infoSignal,help,&Help::infoSlot);
  }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Hide_Show_Stuff(bool tf)
{
    ui->pushButton_HOLD->setEnabled(tf);
    ui->pushButton_Auto_Range->setEnabled(tf);
    ui->pushButton_RATE->setEnabled(tf);
    ui->pushButton_Send->setEnabled(tf);
    ui->pushButton_LOCAL->setEnabled(tf);
    ui->pushButton_20mOhm->setEnabled(tf);
    ui->pushButton_Export->setEnabled(tf);
    ui->pushButton_COMPARE->setEnabled(tf);
    ui->pushButton_Range_UP->setEnabled(tf);
    ui->pushButton_RC_enable->setEnabled(tf);
    ui->pushButton_HIGH_LIMIT->setEnabled(tf);
    ui->pushButton_Range_DOWN->setEnabled(tf);
    ui->pushButton_Kinetic_Start->setEnabled(tf);
    ui->pushButton_NULL_Correction->setEnabled(tf);
    ui->pushButton_Switch_function_set->setEnabled(tf);
    ui->lineEdit_Command->setEnabled(tf);
    ui->lineEdit_File_Name->setEnabled(tf);
}

void MainWindow::Set_Data()
{

}

void MainWindow::Export_Data()
{
    QString filename="Data.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
        {
            QTextStream stream( &file );
            stream << "x" << endl;
        }
    else
    qDebug() << "kann nicht file öffnen";

    qDebug()<< "zu File geschrieben";
}

void MainWindow::on_pushButton_RC_enable_clicked()
{
    //qDebug() << "first button pushed";

    if (ui->pushButton_RC_enable->text() == "Enable")
    {
        //qDebug() << "second button pushed";

        //ui->lineEdit_Command->setText("[+]");
        //serial.write(ui->lineEdit_Command->text().toLocal8Bit());
        serial.write(ActivationCommand.toLocal8Bit());
        ui->pushButton_RC_enable->setText("Disable");
        ui->checkBox_Remote_Control->setChecked(true);

            //HELP MENU PUSH
        /*serial.write(SoftwareVersionCommand.toLocal8Bit());
        serial.waitForReadyRead(50);
        QString SoftwareVersion = serial.readAll();
        
        serial.write(SerialNumberCommand.toLocal8Bit());
        serial.waitForReadyRead(50);
        QString SerialNumber = serial.readAll();
        
        emit infoSignal(SoftwareVersion,SerialNumber);
        */
        
       /* while(true) //BULLSHIT FOR LCD DISPLAY
        {
            serial.write(ValueQuery.toLocal8Bit());
            serial.waitForReadyRead(25);
            ui->Current_Resistance_Display->display(QString(serial.readAll()));
        }*/


        /*
        serial.write("[?R]");
        ui->Current_Range_Display->display(QTextCodec::codecForMib(1015)->toUnicode(serial.readLine()));
        */
    }
    else {
        serial.write(DeactivationCommand.toLocal8Bit());
        ui->pushButton_RC_enable->setText("Enable");
        ui->checkBox_Remote_Control->setChecked(false);
    }
}
void MainWindow::on_pushButton_Connect_clicked()
{
    if (ui->pushButton_Connect->text() == "Connect") {
        serial.setPortName(ui->comboBox_Port_Name->currentText());
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        Hide_Show_Stuff(true);

        if (!serial.open(QIODevice::ReadWrite)) {
            //qDebug()<<"mistake";
            QSerialPort::SerialPortError getError = QSerialPort::NoError;
            serial.error(getError);

            emit response(tr("Can't open %1, error code %2").arg(ui->comboBox_Port_Name->currentText()).arg(serial.error()));

            return;
        }
        //qDebug()<<"no mistake";
        ui->pushButton_Connect->setText("Disconnect");
    } else {
        //qDebug()<<"mistake???";
        serial.close();
        Hide_Show_Stuff(false);

        ui->pushButton_Connect->setText("Connect");

    }
}

void MainWindow::on_pushButton_Range_UP_clicked()
{
    serial.write(RangeQuery.toLocal8Bit());
    serial.waitForReadyRead(25);
    ui->lineEdit_Response->setText(serial.readAll());
    if(ui->lineEdit_Response->text() == "R1")
    {
        serial.write(R2Command.toLocal8Bit());
        return;
    }
    if(ui->lineEdit_Response->text() == "R2")
    {
        serial.write(R3Command.toLocal8Bit());
        return;
    }
    if(ui->lineEdit_Response->text() == "R3")
    {
        serial.write(R4Command.toLocal8Bit());
        return;
    }
    if(ui->lineEdit_Response->text() == "R4")
    {
        serial.write(R5Command.toLocal8Bit());
        return;
    }
    if(ui->lineEdit_Response->text() == "R5")
    {
        serial.write(R6Command.toLocal8Bit());
        return;
    }
    if(ui->lineEdit_Response->text() == "R6")
    {
        serial.write(R7Command.toLocal8Bit());
        return;
    }
    if(ui->lineEdit_Response->text() == "R7")
    {
        serial.write(R8Command.toLocal8Bit());
        return;
    }
    if(ui->lineEdit_Response->text() == "R8")
    {
        serial.write(R9Command.toLocal8Bit());
        return;
    }

}

void MainWindow::on_pushButton_Range_DOWN_clicked()
{
        serial.write(RangeQuery.toLocal8Bit());
        serial.waitForReadyRead(25);
        ui->lineEdit_Response->setText(serial.readAll());
        if(ui->lineEdit_Response->text() == "R9")
        {
            serial.write(R8Command.toLocal8Bit());
            return;
        }
        if(ui->lineEdit_Response->text() == "R8")
        {
            serial.write(R7Command.toLocal8Bit());
            return;
        }
        if(ui->lineEdit_Response->text() == "R7")
        {
            serial.write(R6Command.toLocal8Bit());
            return;
        }
        if(ui->lineEdit_Response->text() == "R6")
        {
            serial.write(R5Command.toLocal8Bit());
            return;
        }
        if(ui->lineEdit_Response->text() == "R5")
        {
            serial.write(R4Command.toLocal8Bit());
            return;
        }
        if(ui->lineEdit_Response->text() == "R4")
        {
            serial.write(R3Command.toLocal8Bit());
            return;
        }
        if(ui->lineEdit_Response->text() == "R3")
        {
            serial.write(R2Command.toLocal8Bit());
            return;
        }
        if(ui->lineEdit_Response->text() == "R2")
        {
            serial.write(R1Command.toLocal8Bit());
            return;
        }
}

void MainWindow::on_pushButton_20mOhm_clicked()
{
    serial.write(R1Command.toLocal8Bit());
}

void MainWindow::on_pushButton_Auto_Range_clicked()
{
    serial.write(AutoRangeCommand.toLocal8Bit());
}

void MainWindow::on_action_triggered()
{
    ui->comboBox_Port_Name->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBox_Port_Name->addItem(info.portName());
    }

    //ui->lineEdit_Response->setText("Clicked!");

    //return;
}

void MainWindow::on_actionHelp_triggered()
{
    help->show();
}

void MainWindow::on_pushButton_Send_clicked()
{
    qDebug() << "send button pushed";
    QString command = ui->lineEdit_Command->text();

    serial.write(command.toLocal8Bit());

}

void MainWindow::timerStart()
{
  time->start();
  Sleep(500);
  ui->lcdNumber_Time->display(QString::number(time->elapsed()));
  qDebug() << "Privet ya zapustilsya";
}

void MainWindow::on_pushButton_Kinetic_Start_clicked()
{
    //this->time->start();
    //ui->lcdNumber_Time->display(QString::number(time->elapsed()));
    //ui->lineEdit_Command->setText(QString::number(time.second()));

   /* while(true)
    {

        serial.write()
    }*/

    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i = 0; i < 101; ++i)
    {
    x[i] = i / 50.0 - 1; // x goes from -1 to 1
    y[i] = x[i] * x[i]; // let's plot a quadratic function
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

    Export_Data();
}

void MainWindow::on_pushButton_HOLD_clicked()
{
        serial.write(HoldQuery.toLocal8Bit());
        if (serial.waitForReadyRead(1000)) {
            ui->lineEdit_Response->setText(serial.readAll());
        } else {
            ui->lineEdit_Response->setText("WTF");
        }
        if(ui->lineEdit_Response->text() == "[G0]")
        {
            serial.write(G1Command.toLocal8Bit());
            ui->checkBox_HOLD->setChecked(true);
            return;
        }
        if(ui->lineEdit_Response->text() == "[G1]")
        {
            serial.write(G0Command.toLocal8Bit());
            ui->checkBox_HOLD->setChecked(false);
            return;
        }
}

void MainWindow::on_pushButton_RATE_clicked()
{
        serial.write(RateQuery.toLocal8Bit());
        Receive();
        ui->lineEdit_Response->setText(serial.readAll());
        if(ui->lineEdit_Response->text() == "F0")
        {
            serial.write(F1Command.toLocal8Bit());
            return;
        }
        if(ui->lineEdit_Response->text() == "F1")
        {
            serial.write(F0Command.toLocal8Bit());
            return;
        }
}

void MainWindow::on_pushButton_NULL_Correction_clicked()
{
            serial.write(NullCorrectionQuery.toLocal8Bit());
            serial.waitForReadyRead(25);
            ui->lineEdit_Response->setText(serial.readAll());
            if(ui->lineEdit_Response->text() == "I0")
            {
                serial.write(I1Command.toLocal8Bit());
                ui->checkBox_AUTO->setChecked(true);
                return;
            }
            if(ui->lineEdit_Response->text() == "I1")
            {
                serial.write(I0Command.toLocal8Bit());
                ui->checkBox_AUTO->setChecked(false);
                return;
            }

}

void MainWindow::on_pushButton_LOCAL_clicked()
{
   QString DeactivationCommand = "[-]";
   serial.write(DeactivationCommand.toLocal8Bit());
   ui->checkBox_Remote_Control->setChecked(false);
   ui->pushButton_RC_enable->setText("Enable");
}

void MainWindow::on_pushButton_COMPARE_clicked()
{
    if(ui->pushButton_COMPARE->text() == "COMPARE")
    {
            serial.write(CompareStatusQuery.toLocal8Bit());
            Receive();
            ui->lineEdit_Response->setText(serial.readAll());
            if(ui->lineEdit_Response->text() == "C0")
            {
                serial.write(C1Command.toLocal8Bit());
                serial.write(CompareResultQuery.toLocal8Bit());
                Receive();
                if(ui->lineEdit_Response->text() == "[M1]")
                {
                    ui->lineEdit_Response->setText("Current Resistance higher than High Limit");
                }
                if(ui->lineEdit_Response->text() == "[M2]")
                {
                    ui->lineEdit_Response->setText("Current Resistance is a Go");
                }
                if(ui->lineEdit_Response->text() == "[M3]")
                {
                    ui->lineEdit_Response->setText("Current Resistance lower than Low Limit");
                }
                return;
            }
            if(ui->lineEdit_Response->text() == "C1")
            {
                serial.write(C0Command.toLocal8Bit());
                return;
            }
    }
    else
    {
            serial.write(BeeperQuery.toLocal8Bit());
            serial.waitForReadyRead(25);
            ui->lineEdit_Response->setText(serial.readAll());
            if(ui->lineEdit_Response->text() == "B0")
            {
                serial.write(B1Command.toLocal8Bit());
                return;
            }
            if(ui->lineEdit_Response->text() == "B1")
            {
                serial.write(B0Command.toLocal8Bit());
                return;
            }
    }

}

/*void MainWindow::on_pushButton_HIGH_LIMIT_clicked()
{
    serial.write(HighLimitQuery.toLocal8Bit());
    Receive();
    QString temp = ui->lineEdit_Response->text();
    temp.remove(0,2);
    temp.toInt();
    emit HighLimitButton(temp);
}

void MainWindow::on_pushButton_ARROW_RIGHT_clicked(const int temp)
{

}*/

void MainWindow::on_pushButton_Switch_function_set_clicked()
{qDebug()<<"switch button pressed";
    if(ui->pushButton_HIGH_LIMIT->text() == "HILM")
    {
        ui->pushButton_HIGH_LIMIT->setText("LOLM");
        ui->pushButton_COMPARE->setText("(|o|)");
       //ui->pushButton_->setText("DEF.");
    }
    else
    {
        ui->pushButton_HIGH_LIMIT->setText("HILM");
        ui->pushButton_COMPARE->setText("COMPARE");
    }
}

void MainWindow::get_response(const QString &s)
{
    ui->lineEdit_Response->setText(s);

    return;
}

void MainWindow::Receive ()
{
    QString temp = "";
    QByteArray array;

    //int WaitTime = ui->spinBox_TimeToWait->value();

    int WaitTime = 500;

    QTime time_to_wait = QTime(0, 0, 0, 0);

    time_to_wait.start();
    if (serial.waitForReadyRead(WaitTime)) {
        array = serial.readAll();
        temp = QString(array);

        while (serial.waitForReadyRead(10)) {
            array = serial.readAll();
            temp += QString(array);
        }
    }
   // ui->lcdNumber_Time->display(QString::number(time.elapsed()));

   /* if (temp.isEmpty()) {
        temp = "Wait write request timeout";
        ui->lcdNumber_Time->display("0000");
    }
    */
    emit response(temp);

    return;

}

void MainWindow::on_pushButton_HIGH_clicked()
{
    QString NewHigh = ui->lineEdit_HIGH_LIMIT->text();
    if(NewHigh.length() > 5)
    {
        ui->lineEdit_Response->setText("Error. The format for High Limit is of following: XXXXX");
    }
    while(NewHigh.length() != 5)
    {
        NewHigh.prepend("0");
    }
    QString HighCommand = "[H="+NewHigh+"]";
    serial.write(HighCommand.toLocal8Bit());
}

void MainWindow::on_pushButton_LOW_clicked()
{
    QString NewLow = ui->lineEdit_LOW_LIMIT->text();
    if(NewLow.length() > 5)
    {
        ui->lineEdit_Response->setText("Error. The format for High Limit is of following: XXXXX");
    }
    while(NewLow.length() != 5)
    {
        NewLow.prepend("0");
    }
    QString LowCommand = "[H="+NewLow+"]";
    serial.write(LowCommand.toLocal8Bit());
}
