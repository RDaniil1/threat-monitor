#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    void copyValues(QString& host, QString& port,
                    QString& user, QString& password,
                    QString& dbName);

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
