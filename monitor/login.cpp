#include "login.h"
#include "./ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEditPass->setEchoMode(QLineEdit::NoEcho);
}

Login::~Login()
{
    delete ui;
}

void Login::copyValues(QString& host, QString& port,
                        QString& user, QString& password,
                        QString& dbName)
{
    host = ui->lineEditHost->text();
    port = ui->lineEditPort->text();
    user = ui->lineEditUsername->text();
    password = ui->lineEditPass->text();
    dbName = ui->lineEditDbName->text();
}
