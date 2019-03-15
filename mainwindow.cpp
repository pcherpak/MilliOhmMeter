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
    ui->lineEdit_x_point->setEnabled(false);
    ui->lineEdit_y_point->setEnabled(false);

    //ui->pushButton_Send->setEnabled(false);

    //ui->pushButton_Export->setEnabled(false);

    //ui->checkBox_Remote_Control->setVisible(false);
    ui->checkBox_Remote_Control->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBox_Remote_Control->setFocusPolicy(Qt::NoFocus);
    ui->checkBox_AUTO->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBox_AUTO->setFocusPolicy(Qt::NoFocus);
    ui->checkBox_NULL->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBox_NULL->setFocusPolicy(Qt::NoFocus);
    ui->checkBox_HOLD->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBox_HOLD->setFocusPolicy(Qt::NoFocus);
    ui->checkBox_Bleeper->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBox_Bleeper->setFocusPolicy(Qt::NoFocus);
    ui->checkBox_SLOW->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBox_SLOW->setFocusPolicy(Qt::NoFocus);
    ui->checkBox_FAST->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBox_FAST->setFocusPolicy(Qt::NoFocus);

    //connect (this, Signal(stop_signal(QString), this, SLOT(process_stop_signal(QString));))

    connect(this, SIGNAL(response(QString)), this, SLOT(get_response(QString)));
    time = new QTime();
    //connect(ui->pushButton_Kinetic_Start, SIGNAL(released()), this, SLOT(timerStart()));

    help = new(Help); //Help Window Created
    connect(this,&MainWindow::infoSignal,help,&Help::infoSlot);

    BOOOL = false;

    connect(ui->customPlot_Resistance, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_customPlot_Resistance_Moved(QMouseEvent*)));
}

MainWindow::~MainWindow()
{
    //serial.write(DeactivationCommand.toLocal8Bit());
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
    ui->pushButton_COMPARE->setEnabled(tf);
    ui->pushButton_Range_UP->setEnabled(tf);
    ui->pushButton_RC_enable->setEnabled(tf);
    ui->pushButton_Range_DOWN->setEnabled(tf);
    ui->pushButton_Kinetic_Start->setEnabled(tf);
    ui->pushButton_NULL_Correction->setEnabled(tf);
    ui->pushButton_HIGH->setEnabled(tf);
    ui->pushButton_LOW->setEnabled(tf);
    ui->pushButton_Bleeper->setEnabled(tf);
    ui->lineEdit_Command->setEnabled(tf);
    ui->lineEdit_LOW_LIMIT->setEnabled(tf);
    ui->lineEdit_HIGH_LIMIT->setEnabled(tf);
}


void MainWindow::on_pushButton_RC_enable_clicked()
{

    if (ui->pushButton_RC_enable->text() == "Enable")
    {

        serial.write(ActivationCommand.toLocal8Bit());
        ui->pushButton_RC_enable->setText("Disable");
        ui->checkBox_Remote_Control->setChecked(true);


        serial.write(HighLimitQuery.toLocal8Bit());
        Receive();
        QString High = ui->lineEdit_Response->text();
        High.chop(1);
        High.remove(0,7);
        while (High.length() != 5)
        {
            High.remove(0,1);
        }
        ui->lineEdit_HIGH_LIMIT->setText(High);

        serial.write(LowLimitQuery.toLocal8Bit());
        Receive();
        QString Low = ui->lineEdit_Response->text();
        Low.chop(1);
        Low.remove(0,3);
        ui->lineEdit_LOW_LIMIT->setText(Low);

        Read_Settings();

        //HELP MENU PUSH
        serial.write(SoftwareVersionCommand.toLocal8Bit());
        Receive();
        QString SoftwareVersion = ui->lineEdit_Response->text();
        
        //LIMITS SETTING PULL
        serial.write(SerialNumberCommand.toLocal8Bit());
        Receive();
        QString SerialNumber = ui->lineEdit_Response->text();
        
        emit infoSignal(SoftwareVersion,SerialNumber);

        
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
        serial.setParity(QSerialPort::NoParity);
        serial.setDataBits(QSerialPort::Data8);
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
        serial.write(DeactivationCommand.toLocal8Bit());
        ui->pushButton_Connect->setText("Disconnect");
    } else {
        //qDebug()<<"mistake???";
        serial.close();
        Hide_Show_Stuff(false);

        ui->pushButton_Connect->setText("Connect");

    }
}

/*void MainWindow::Export_Data(double x, double y)
{
    QString filename="Data.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
        {
            QTextStream stream( &file );
            stream << x << ",   " << y << endl;
        }
    else
    qDebug() << "kann nicht file öffnen";

    qDebug()<< "zu File geschrieben";
}*/

void MainWindow::on_customPlot_Resistance_Moved(QMouseEvent *event)
{
    ui->lineEdit_x_point->setText(QString::number(ui->customPlot_Resistance->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEdit_y_point->setText(QString::number(ui->customPlot_Resistance->yAxis->pixelToCoord(event->pos().y())));

    return;
}
void MainWindow::on_pushButton_Kinetic_Start_clicked()
{
    x.clear();
    y.clear();
    Hide_Show_Stuff(false);
    ui->pushButton_Kinetic_Start->setEnabled(true);
    //QString Data = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
    //QString filename=Data + ".txt";
    //QString filename="Data.txt";
    //QFile file( filename );
    //file.open(QIODevice::WriteOnly);
    //QTextStream stream( &file );
    //stream << "Time" << "      " << "Resistance" << endl;
    //stream << "seconds" << "       " << "Ohm" << endl;
    ui->customPlot_Resistance->addGraph();
    ui->customPlot_Resistance->xAxis->setLabel("Time (s)");
    ui->customPlot_Resistance->yAxis->setLabel("Resistance (Ohm)");
    ui->customPlot_Resistance->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    if (ui->pushButton_Kinetic_Start->text() == "Kinetic Start") {
        QString Data = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
        QString filename=Data + ".txt";
        QFile file( filename );
        file.open(QIODevice::WriteOnly);
        QTextStream stream( &file );
        stream << "Time" << "      " << "Resistance" << endl;
        stream << "seconds" << "       " << "Ohm" << endl;
        BOOOL = true;
        ui->pushButton_Kinetic_Start->setText("Kinetic Stop");
        this->time->start();
        //Sleep(500);
        //ui->lcdNumber_Time->display(QString::number(time->elapsed()));
    //ui->lineEdit_Command->setText(QString::number(time.second()));


        while (BOOOL)
            {
            serial.write(ValueQuery.toLocal8Bit());
            Receive();
            QString CurrentResistance = ui->lineEdit_Response->text();

            if(CurrentResistance.contains("mOhm"))
            {
                CurrentResistance.chop(5);
                CurrentResistance.remove(0,3);
                //CurrentResistance.remove(0,1);
                double CurrentResistance_double = CurrentResistance.toDouble();
                //qDebug()<<CurrentResistance;
               // qDebug()<<CurrentResistance_double;
                CurrentResistance_double = CurrentResistance_double/1000;
                //qDebug()<<CurrentResistance_double;
                double NewTime_mOhm = time->elapsed();
                NewTime_mOhm = NewTime_mOhm/1000;
                QTextStream stream( &file );
                stream << NewTime_mOhm << "     " << CurrentResistance_double << endl;
                x.push_back(NewTime_mOhm);
                y.push_back(CurrentResistance_double);
                ui->customPlot_Resistance->graph(0)->setData(x, y);
                ui->customPlot_Resistance->rescaleAxes();
                ui->customPlot_Resistance->rescaleAxes();
                ui->customPlot_Resistance->replot();

            }
            if(CurrentResistance.contains("KOhm"))
            {
                //qDebug()<<Kinetic_Condition(CurrentResistance);
                CurrentResistance.chop(5);
                CurrentResistance.remove(0,3);
                double CurrentResistance_double = CurrentResistance.toDouble();
                CurrentResistance_double = CurrentResistance_double * 1000;
                //qDebug()<<CurrentResistance;
                //qDebug()<<CurrentResistance_double;
                double NewTime_kOhm = time->elapsed();
                //qDebug()<<CurrentResistance_double;
                NewTime_kOhm = NewTime_kOhm/1000;
                QTextStream stream( &file );
                stream << NewTime_kOhm << "     " << CurrentResistance_double << endl;
                x.push_back(NewTime_kOhm);
                y.push_back(CurrentResistance_double);
                ui->customPlot_Resistance->graph(0)->setData(x, y);
                ui->customPlot_Resistance->rescaleAxes();
                ui->customPlot_Resistance->rescaleAxes();
                ui->customPlot_Resistance->replot();
            }
            if(CurrentResistance.contains("Ohm"))
            {
                CurrentResistance.chop(4);
                CurrentResistance.remove(0,3);
                double CurrentResistance_double = CurrentResistance.toDouble();
                CurrentResistance_double = CurrentResistance_double/1000;
                qDebug()<<CurrentResistance_double;
                double NewTime_Ohm = time->elapsed();
                NewTime_Ohm = NewTime_Ohm/1000;
                QTextStream stream( &file );
                stream << NewTime_Ohm << "      " << CurrentResistance_double << endl;
                x.push_back(NewTime_Ohm);
                y.push_back(CurrentResistance_double);
                ui->customPlot_Resistance->graph(0)->setData(x, y);
                ui->customPlot_Resistance->rescaleAxes();
                ui->customPlot_Resistance->rescaleAxes();
                ui->customPlot_Resistance->replot();
            }

            if(CurrentResistance.contains("MOhm"))
            {
                CurrentResistance.chop(5);
                CurrentResistance.remove(0,2);
                double CurrentResistance_double = CurrentResistance.toDouble();
                CurrentResistance_double = CurrentResistance_double * 1000000;
                double NewTime_MOhm = time->elapsed();
                NewTime_MOhm = NewTime_MOhm/1000;
                QTextStream stream( &file );
                stream << NewTime_MOhm << "     " << CurrentResistance_double << endl;
                x.push_back(NewTime_MOhm);
                y.push_back(CurrentResistance_double);
                ui->customPlot_Resistance->graph(0)->setData(x, y);
                ui->customPlot_Resistance->rescaleAxes();
                ui->customPlot_Resistance->rescaleAxes();
                ui->customPlot_Resistance->replot();
            }
            QApplication::processEvents();
            }

    // EXAMPLE data:
    /*QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i = 0; i < 101; ++i)
    {
    x[i] = i / 50.0 - 1; // x goes from -1 to 1
    y[i] = x[i] * x[i];

    QTextStream stream( &file );
    stream << x[i] << "     " << y[i] << endl;

    //qDebug() << "kann nicht file öffnen";

    //qDebug() << "zu File geschrieben";

    //Export_Data( x[i], y[i]);
    // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->customPlot_Resistance->graph(0)->setData(x, y);
    // give the axes some labels:
    // set axes ranges, so we see all data:
    ui->customPlot_Resistance->xAxis->setRange(-1, 1);
    ui->customPlot_Resistance->yAxis->setRange(0, 1);
    ui->customPlot_Resistance->replot();*/
} else
    {
        Hide_Show_Stuff(true);
        BOOOL = false;
        //file.close();
        ui->pushButton_Kinetic_Start->setText("Kinetic Start");
    }

}

/*void MainWindow::timerStart()
{
  time->start();
  Sleep(500);
  ui->lcdNumber_Time->display(QString::number(time->elapsed()));
  qDebug() << "Privet ya zapustilsya";
}*/

void MainWindow::on_pushButton_Range_UP_clicked()
{
    serial.write(RangeQuery.toLocal8Bit());
    Receive();
//    ui->lineEdit_Command->setText(RangeQuery);
//    on_pushButton_Send_clicked();
//    return;

    //serial.waitForReadyRead(25);
    //if(ui->lineEdit_Response->text() == "[R1]")
    if(ui->lineEdit_Response->text().contains("[R1]"))
    {
        serial.write(R2Command.toLocal8Bit());
        ui->checkBox_AUTO->setChecked(false);
        return;
    }
    //if(ui->lineEdit_Response->text() == "[R2]")
    if(ui->lineEdit_Response->text().contains("[R2]"))
    {
        serial.write(R3Command.toLocal8Bit());
        ui->checkBox_AUTO->setChecked(false);
        return;
    }
    //if(ui->lineEdit_Response->text() == "[R3]")
    if(ui->lineEdit_Response->text().contains("[R3]"))
    {
        //ui->lineEdit_Command->setText("Ok");
        serial.write(R4Command.toLocal8Bit());
        ui->checkBox_AUTO->setChecked(false);
        return;
    }
    //if(ui->lineEdit_Response->text() == "[R4]")
    if(ui->lineEdit_Response->text().contains("[R4]"))
    {
        serial.write(R5Command.toLocal8Bit());
        ui->checkBox_AUTO->setChecked(false);
        return;
    }
    //if(ui->lineEdit_Response->text() == "[R5]")
    if(ui->lineEdit_Response->text().contains("[R5]"))
    {
        serial.write(R6Command.toLocal8Bit());
        ui->checkBox_AUTO->setChecked(false);
        return;
    }
    //if(ui->lineEdit_Response->text() == "[R6]")
    if(ui->lineEdit_Response->text().contains("[R6]"))
    {
        serial.write(R7Command.toLocal8Bit());
        ui->checkBox_AUTO->setChecked(false);
        return;
    }
    //if(ui->lineEdit_Response->text() == "[R7]")
    if(ui->lineEdit_Response->text().contains("[R7]"))
    {
        serial.write(R8Command.toLocal8Bit());
        ui->checkBox_AUTO->setChecked(false);
        return;
    }
    //if(ui->lineEdit_Response->text() == "[R8]")
    if(ui->lineEdit_Response->text().contains("[R8]"))
    {
        serial.write(R9Command.toLocal8Bit());
        ui->checkBox_AUTO->setChecked(false);
        return;
    }

}

void MainWindow::on_pushButton_Range_DOWN_clicked()
{
        serial.write(RangeQuery.toLocal8Bit());
        Receive();
        //serial.waitForReadyRead(25);
        //ui->lineEdit_Response->setText(serial.readAll());
        //if(ui->lineEdit_Response->text() == "[R9]")
        if(ui->lineEdit_Response->text().contains("[R9]"))
        {
            serial.write(R8Command.toLocal8Bit());
            ui->checkBox_AUTO->setChecked(false);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[R8]")
        if(ui->lineEdit_Response->text().contains("[R8]"))
        {
            serial.write(R7Command.toLocal8Bit());
            ui->checkBox_AUTO->setChecked(false);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[R7]")
        if(ui->lineEdit_Response->text().contains("[R7]"))
        {
            serial.write(R6Command.toLocal8Bit());
            ui->checkBox_AUTO->setChecked(false);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[R6]")
        if(ui->lineEdit_Response->text().contains("[R6]"))
        {
            serial.write(R5Command.toLocal8Bit());
            ui->checkBox_AUTO->setChecked(false);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[R5]")
        if(ui->lineEdit_Response->text().contains("[R5]"))
        {
            serial.write(R4Command.toLocal8Bit());
            ui->checkBox_AUTO->setChecked(false);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[R4]")
        if(ui->lineEdit_Response->text().contains("[R4]"))
        {
            serial.write(R3Command.toLocal8Bit());
            ui->checkBox_AUTO->setChecked(false);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[R3]")
        if(ui->lineEdit_Response->text().contains("[R3]"))
        {
            serial.write(R2Command.toLocal8Bit());
            ui->checkBox_AUTO->setChecked(false);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[R2]")
        if(ui->lineEdit_Response->text().contains("[R2]"))
        {
            serial.write(R1Command.toLocal8Bit());
            ui->checkBox_AUTO->setChecked(false);
            return;
        }
}

void MainWindow::on_pushButton_20mOhm_clicked()
{
    serial.write(R1Command.toLocal8Bit());
    ui->checkBox_AUTO->setChecked(false);
}

void MainWindow::on_pushButton_Auto_Range_clicked()
{
    serial.write(AutoRangeCommand.toLocal8Bit());
    ui->checkBox_AUTO->setChecked(true);
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

void MainWindow::on_pushButton_HOLD_clicked()
{
        serial.write(HoldQuery.toLocal8Bit());
        Receive();
        /*if (serial.waitForReadyRead(1000)) {
            ui->lineEdit_Response->setText(serial.readAll());
        } else {
            ui->lineEdit_Response->setText("WTF");
        }*/
        //if(ui->lineEdit_Response->text() == "[G0]")
        if(ui->lineEdit_Response->text().contains("[G0]"))
        {
            serial.write(G1Command.toLocal8Bit());
            ui->checkBox_HOLD->setChecked(true);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[G1]")
        if(ui->lineEdit_Response->text().contains("[G1]"))
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
        //ui->lineEdit_Response->setText(serial.readAll());
        //if(ui->lineEdit_Response->text() == "[F0]")
        if(ui->lineEdit_Response->text().contains("[F0]"))
        {
            serial.write(F1Command.toLocal8Bit());
            ui->checkBox_SLOW->setChecked(true);
            ui->checkBox_FAST->setChecked(false);
            return;
        }
        //if(ui->lineEdit_Response->text() == "[F1]")
        if(ui->lineEdit_Response->text().contains("[F1]"))
        {
            serial.write(F0Command.toLocal8Bit());
            ui->checkBox_SLOW->setChecked(false);
            ui->checkBox_FAST->setChecked(true);
            return;
        }
}

void MainWindow::on_pushButton_NULL_Correction_clicked()
{
            serial.write(NullCorrectionQuery.toLocal8Bit());
            Receive();
            //serial.waitForReadyRead(25);
            //ui->lineEdit_Response->setText(serial.readAll());
            //if(ui->lineEdit_Response->text() == "[I0]")
            if(ui->lineEdit_Response->text().contains("[I0]"))
            {
                serial.write(I1Command.toLocal8Bit());
                ui->checkBox_NULL->setChecked(true);
                return;
            }
            //if(ui->lineEdit_Response->text() == "[I1]")
            if(ui->lineEdit_Response->text().contains("[I1]"))
            {
                serial.write(I0Command.toLocal8Bit());
                ui->checkBox_NULL->setChecked(false);
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
    //if(ui->pushButton_COMPARE->text() == "COMPARE")
    //{
            serial.write(CompareStatusQuery.toLocal8Bit());
            Receive();
            //ui->lineEdit_Response->setText(serial.readAll());
            //if(ui->lineEdit_Response->text() == "[C0]")
            if(ui->lineEdit_Response->text().contains("[C0]"))
            {
                serial.write(C1Command.toLocal8Bit());
                serial.write(CompareResultQuery.toLocal8Bit());
                Receive();
                //if(ui->lineEdit_Response->text() == "[M1]")
                if(ui->lineEdit_Response->text().contains("[M1]"))
                {
                    ui->lineEdit_Response->setText("Current Resistance higher than High Limit");
                }
                //if(ui->lineEdit_Response->text() == "[M2]")
                if(ui->lineEdit_Response->text().contains("[M2]"))
                {
                    ui->lineEdit_Response->setText("Current Resistance is a Go");
                }
                //if(ui->lineEdit_Response->text() == "[M3]")
                if(ui->lineEdit_Response->text().contains("[M3]"))
                {
                    ui->lineEdit_Response->setText("Current Resistance lower than Low Limit");
                }
                return;
            }
            //if(ui->lineEdit_Response->text() == "[C1]")
            if(ui->lineEdit_Response->text().contains("[C1]"))
            {
                serial.write(C0Command.toLocal8Bit());
                return;
            }
    //}
    /*else
    {
            serial.write(BeeperQuery.toLocal8Bit());
            Receive();
            //ui->lineEdit_Response->setText(serial.readAll());
            //if(ui->lineEdit_Response->text() == "[B0]")
            if(ui->lineEdit_Response->text().contains("[B0]"))
            {
                serial.write(B1Command.toLocal8Bit());
                ui->checkBox_Bleeper->setChecked(true);
                return;
            }
            //if(ui->lineEdit_Response->text() == "[B1]")
            if(ui->lineEdit_Response->text().contains("[B1]"))
            {
                serial.write(B0Command.toLocal8Bit());
                ui->checkBox_Bleeper->setChecked(false);
                return;
            }
    }*/

}
void MainWindow::on_pushButton_Bleeper_clicked()
{
    serial.write(BeeperQuery.toLocal8Bit());
    Receive();
    //ui->lineEdit_Response->setText(serial.readAll());
    //if(ui->lineEdit_Response->text() == "[B0]")
    if(ui->lineEdit_Response->text().contains("[B0]"))
    {
        serial.write(B1Command.toLocal8Bit());
        ui->checkBox_Bleeper->setChecked(true);
        return;
    }
    //if(ui->lineEdit_Response->text() == "[B1]")
    if(ui->lineEdit_Response->text().contains("[B1]"))
    {
        serial.write(B0Command.toLocal8Bit());
        ui->checkBox_Bleeper->setChecked(false);
        return;
    }
}

void MainWindow::Read_Settings()
{
    serial.write(RangeQuery.toLocal8Bit());
    Receive();
    if(ui->lineEdit_Response->text().contains("R01"))
        ui->checkBox_AUTO->setChecked(true);
    else {
        ui->checkBox_AUTO->setChecked(false);
    }
    serial.write(HoldQuery.toLocal8Bit());
    Receive();
    if(ui->lineEdit_Response->text().contains("G1"))
        ui->checkBox_HOLD->setChecked(true);
    else {
        ui->checkBox_HOLD->setChecked(false);
    }
    serial.write(NullCorrectionQuery.toLocal8Bit());
    Receive();
    if(ui->lineEdit_Response->text().contains("I1"))
        ui->checkBox_NULL->setChecked(true);
    else {
        ui->checkBox_NULL->setChecked(false);
    }
    serial.write(BeeperQuery.toLocal8Bit());
    Receive();
    if(ui->lineEdit_Response->text().contains("B1"))
        ui->checkBox_Bleeper->setChecked(true);
    else {
        ui->checkBox_Bleeper->setChecked(false);
    }
    serial.write(RateQuery.toLocal8Bit());
    Receive();
    if(ui->lineEdit_Response->text().contains("F1"))
        ui->checkBox_SLOW->setChecked(true);
    else {
        ui->checkBox_FAST->setChecked(true);
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

/*void MainWindow::on_pushButton_Switch_function_set_clicked()
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
}*/



void MainWindow::on_pushButton_HIGH_clicked()
{
    QString NewHigh = ui->lineEdit_HIGH_LIMIT->text();
    if(NewHigh.length() > 5)
    {
        ui->lineEdit_Response->setText("Error. The format for High Limit is of following: XXXXX");
        return;
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
        ui->lineEdit_Response->setText("Error. The format for Low Limit is of following: XXXXX");
        return;
    }
    while(NewLow.length() != 5)
    {
        NewLow.prepend("0");
    }
    QString LowCommand = "[L="+NewLow+"]";
    serial.write(LowCommand.toLocal8Bit());
}

//-----------------------------------------------------COMMUNICATION BEGIN-------------------------------------------------------------//
void MainWindow::on_pushButton_Send_clicked()
{
    qDebug() << "send button pushed";
    QString command = ui->lineEdit_Command->text();

    serial.write(command.toLocal8Bit());

    Receive();
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

        while (serial.waitForReadyRead(150)) {
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
    emit response(temp.trimmed());

    return;

}
//-----------------------------------------------------COMMUNICATION END-------------------------------------------------------------//




