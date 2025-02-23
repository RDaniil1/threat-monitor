#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <algorithm>

#include <QMainWindow>

#include <QSqlDatabase>
#include <QSqlError>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlQuery>
#include <QtSql>

#include <QVector>
#include <QString>
#include <QRegularExpression>
#include <QThread>
#include <QChart>
#include <QPieSeries>
#include <QChartView>
#include <QThread>

#include <dotenv.h>

#include "qcustomplot.h"

#include "config.hpp"
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool initConnection();
    bool initConnection(QString ip, QString port, QString user, QString password, QString dbName);
    void initQuery();   

    void execQuery(QString query);
    
    void setTableData();
    void updateTableData();

    void setChartData();
    void plotChart();
    void updateChart();

    void setPlottingData();
    void setPlotGathered();
    void setPlotPredicted();

    void plotNetworkUsage();
    void configurePlot();
    void plotNetworkGathered();
    void plotNetworkPredicted();

    void updatePlot();
    
    void showLoginDialog();

    QVector<qreal> time, usage;
    QVector<qreal> timePred, usagePred;

    QVector<QString> labels;
    QVector<qreal> percentages;

    QSqlDatabase db;
    QSqlQuery* query;
    QSqlRecord record;
    QStandardItemModel* qStandardItemModel;
    Ui::MainWindow *ui;

    QTimer* timer;
    
    Login* login;
    QString ip, port, user, password, dbName;

    size_t gatheredSize = 50;
};
#endif // MAINWINDOW_H
