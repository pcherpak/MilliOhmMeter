#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    on_action_COM_Port_Update_triggered();

    Hide_Show_Stuff(false);

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
    ui->pushButton_RC_enable->setEnabled(false);

    //connect (this, Signal(stop_signal(QString), this, SLOT(process_stop_signal(QString));))

    connect(this, SIGNAL(response(QString)), this, SLOT(get_response(QString)));
    time = new QTime();
    //connect(ui->pushButton_Kinetic_Start, SIGNAL(released()), this, SLOT(timerStart()));

    help = new(Help); //Help Window Created
    milliOhmMeter = new(MilliOhmMeter);
    connect(this,&MainWindow::infoSignal,help,&Help::infoSlot);
    connect(milliOhmMeter, &MilliOhmMeter::error,this,&MainWindow::get_response);

    BOOOL = false;

    connect(ui->customPlot_Resistance, &QCustomPlot::mouseMove, this, &MainWindow::on_customPlot_Resistance_Moved);
    ui->customPlot_Resistance->addGraph();
    ui->customPlot_Resistance->xAxis->setLabel("Time (s)");
    ui->customPlot_Resistance->yAxis->setLabel("Resistance (Ohm)");
    ui->customPlot_Resistance->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

MainWindow::~MainWindow()
{
    delete milliOhmMeter;
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
    ui->pushButton_Range_DOWN->setEnabled(tf);
    ui->pushButton_Kinetic_Start->setEnabled(tf);
    ui->pushButton_NULL_Correction->setEnabled(tf);
    ui->pushButton_HIGH->setEnabled(tf);
    ui->pushButton_LOW->setEnabled(tf);
    ui->pushButton_Bleeper->setEnabled(tf);
    ui->lineEdit_Command->setEnabled(tf);
    ui->lineEdit_LOW_LIMIT->setEnabled(tf);
    ui->lineEdit_HIGH_LIMIT->setEnabled(tf);
    ui->lineEdit_Response->setEnabled(tf);
    ui->lineEdit_x_point->setEnabled(tf);
    ui->lineEdit_y_point->setEnabled(tf);
    ui->lineEdit_compare->setEnabled(tf);
}

void MainWindow::on_pushButton_RC_enable_clicked()
{

    if (ui->pushButton_RC_enable->text() == "Enable")
    {
        milliOhmMeter->activation();
        ui->pushButton_RC_enable->setText("Disable");
        ui->checkBox_Remote_Control->setChecked(true);
        Hide_Show_Stuff(true);
        Read_Settings();
    }
    else {
        milliOhmMeter->deactivation();
        ui->pushButton_RC_enable->setText("Enable");
        ui->checkBox_Remote_Control->setChecked(false);
        Hide_Show_Stuff(false);
    }
}

void MainWindow::on_pushButton_Connect_clicked()
{
    if (ui->pushButton_Connect->text() == "Connect") {
        if (!milliOhmMeter->connectSerial(ui->comboBox_Port_Name->currentText())) {
            emit response(tr("Can't open %1, error code %2").arg(ui->comboBox_Port_Name->currentText()).arg(milliOhmMeter->serialError()));
            return;
        }
        if(milliOhmMeter->serialNumber() == "") {
            milliOhmMeter->disconnectSerial();
            emit response("Another device. Change COM port");
            return;
        }
        ui->pushButton_Connect->setText("Disconnect");
        ui->pushButton_RC_enable->setEnabled(true);
        milliOhmMeter->deactivation();
    } else {
        on_pushButton_RC_enable_clicked();
        milliOhmMeter->disconnectSerial();
        ui->pushButton_RC_enable->setEnabled(false);
        ui->pushButton_Connect->setText("Connect");
    }
}

void MainWindow::on_customPlot_Resistance_Moved(QMouseEvent *event)
{
    ui->lineEdit_x_point->setText(QString::number(ui->customPlot_Resistance->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEdit_y_point->setText(QString::number(ui->customPlot_Resistance->yAxis->pixelToCoord(event->pos().y())));
}

void MainWindow::get_response(QString s)
{
    ui->lineEdit_Response->setText(s);
}
void MainWindow::on_pushButton_Kinetic_Start_clicked()
{
    x.clear();
    y.clear();
    Hide_Show_Stuff(false);
    ui->pushButton_Kinetic_Start->setEnabled(true);
    if (ui->pushButton_Kinetic_Start->text() == "Kinetic Start") {
        QString Data = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
        QString filename=Data + ".txt";
        QFile file( filename );
        file.open(QIODevice::WriteOnly);
        QTextStream stream( &file );
        stream << "Time" << "\t" << "Resistance" << endl;
        stream << "seconds" << "\t" << "Ohm" << endl;
        BOOOL = true;
        ui->pushButton_Kinetic_Start->setText("Kinetic Stop");
        this->time->start();
        while (BOOOL) {
            double newTime = time->elapsed();
            newTime = newTime/1000;
            double value = milliOhmMeter->value();
            if(value == 0)
                continue;
            stream << newTime << "\t" << value << endl;
            x.push_back(newTime);
            y.push_back(value);
            ui->customPlot_Resistance->graph(0)->setData(x, y);
            ui->customPlot_Resistance->rescaleAxes();
            ui->customPlot_Resistance->rescaleAxes();
            ui->customPlot_Resistance->replot();
            QApplication::processEvents();
        }
        file.close();
    } else {
        Hide_Show_Stuff(true);
        BOOOL = false;
        ui->pushButton_Kinetic_Start->setText("Kinetic Start");
    }

}

void MainWindow::on_actionHelp_triggered()
{
    help->show();
}

void MainWindow::on_pushButton_Range_UP_clicked()
{
    milliOhmMeter->upRange();
}

void MainWindow::on_pushButton_Range_DOWN_clicked()
{
    milliOhmMeter->downRange();
}

void MainWindow::on_pushButton_20mOhm_clicked()
{
    milliOhmMeter->range20mOhm();
    ui->checkBox_AUTO->setChecked(false);
}

void MainWindow::on_pushButton_Auto_Range_clicked()
{
    milliOhmMeter->autoRange(true);
    ui->checkBox_AUTO->setChecked(true);
}

void MainWindow::on_pushButton_HOLD_clicked()
{
    milliOhmMeter->hold(ui->checkBox_HOLD->isChecked());
    ui->checkBox_HOLD->toggle();
}

void MainWindow::on_pushButton_RATE_clicked()
{
    milliOhmMeter->rate(ui->checkBox_FAST->isChecked());
    ui->checkBox_FAST->toggle();
    ui->checkBox_SLOW->toggle();
}

void MainWindow::on_pushButton_NULL_Correction_clicked()
{
    milliOhmMeter->nullCollection(!ui->checkBox_NULL->isChecked());
    ui->checkBox_NULL->toggle();
}

void MainWindow::on_pushButton_LOCAL_clicked()
{
   milliOhmMeter->local();
   ui->checkBox_Remote_Control->setChecked(false);
   ui->pushButton_RC_enable->setText("Enable");
}

void MainWindow::on_pushButton_COMPARE_clicked()
{
    if(ui->pushButton_COMPARE->text() == "COMPARE ON")
    {
        ui->pushButton_COMPARE->setText("COMPARE OFF");
        Hide_Show_Stuff(false);
        ui->pushButton_COMPARE->setEnabled(true);
        ui->lineEdit_compare->setEnabled(true);
        milliOhmMeter->compare(true);
        while(ui->pushButton_COMPARE->text() == "COMPARE OFF") {
            ui->lineEdit_compare->setText(milliOhmMeter->compareResult());
            QApplication::processEvents();
        }
    }
    else {
        milliOhmMeter->compare(false);
        ui->pushButton_COMPARE->setText("COMPARE ON");
        Hide_Show_Stuff(true);
    }
}

void MainWindow::on_pushButton_Bleeper_clicked()
{
    milliOhmMeter->beeper(!ui->checkBox_Bleeper->isChecked());
    ui->checkBox_Bleeper->toggle();
}

void MainWindow::Read_Settings()
{
    ui->checkBox_AUTO->setChecked(milliOhmMeter->autoRange());
    ui->checkBox_HOLD->setChecked(milliOhmMeter->hold());
    ui->checkBox_NULL->setChecked(milliOhmMeter->nullCollection());
    ui->checkBox_Bleeper->setChecked(milliOhmMeter->beeper());
    ui->checkBox_SLOW->setChecked(milliOhmMeter->rate());
    ui->checkBox_FAST->setChecked(!milliOhmMeter->rate());
    ui->lineEdit_LOW_LIMIT->setText(milliOhmMeter->lowLimit());
    ui->lineEdit_HIGH_LIMIT->setText(milliOhmMeter->highLimit());
    if(milliOhmMeter->compare())
        ui->pushButton_COMPARE->setText("COMPARE ON");
    else
        ui->pushButton_COMPARE->setText("COMPARE OFF");
    emit infoSignal(milliOhmMeter->softwareVersion(),milliOhmMeter->serialNumber());
}

void MainWindow::on_pushButton_HIGH_clicked()
{
    milliOhmMeter->highLimit(ui->lineEdit_HIGH_LIMIT->text());
}

void MainWindow::on_pushButton_LOW_clicked()
{
    milliOhmMeter->lowLimit(ui->lineEdit_LOW_LIMIT->text());
}

void MainWindow::on_pushButton_Send_clicked()
{
    milliOhmMeter->send(ui->lineEdit_Command->text());
    ui->lineEdit_Response->setText(milliOhmMeter->receive());
}

void MainWindow::on_action_COM_Port_Update_triggered()
{
    ui->comboBox_Port_Name->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBox_Port_Name->addItem(info.portName());
    }
}

void MainWindow::on_pushButton_Choose_File_for_Export_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File with Data"), "",
            tr("Text Files (*.txt);; All Files (*.*)"));
    ui->lineEdit_Export_File_Header->setText(fileName);
}

void MainWindow::on_pushButton_import_clicked()
{
    QFile file(ui->lineEdit_Export_File_Header->text());
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        ui->lineEdit_Response->setText("Can't open file");
        return;
    }
    QTextStream in(&file);
    in.readLine();
    in.readLine();
    QVector<double> xTemp;
    QVector<double> yTemp;
    while(!in.atEnd()) {
        QString temp = in.readLine();
        QStringList temp2 = temp.split("\t");
        xTemp.push_back(temp2[0].toDouble());
        yTemp.push_back(temp2[1].toDouble());
    }
    ui->customPlot_Resistance->graph(0)->setData(xTemp,yTemp);
    ui->customPlot_Resistance->rescaleAxes();
    ui->customPlot_Resistance->replot();

}
