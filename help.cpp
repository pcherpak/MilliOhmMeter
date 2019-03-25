#include "help.h"
#include "ui_help.h"

Help::Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);

    ui->lineEdit_Software_Version->setEnabled(false);
    ui->lineEdit_Serial_Number->setEnabled(false);
}

Help::~Help()
{
    delete ui;
}

void Help::infoSlot(const QString &Software,const QString &Serial)
{
    ui->lineEdit_Software_Version->setText(Software);
    ui->lineEdit_Serial_Number->setText(Serial);
}
