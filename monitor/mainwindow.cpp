#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Threat monitoring system");
    ui->tableView->setFixedSize(770, 800);

    db = QSqlDatabase::addDatabase("QPSQL");
    if (int noConnection; DEVELOPER_MODE)
    {
        dotenv::init(ENV_PATH);
        noConnection = initConnection(std::getenv("host"), std::getenv("port"), std::getenv("user"), 
                       std::getenv("password"), std::getenv("dbname"));
        if (noConnection)
        {
            qDebug() << "Can't establish connection to PostgreSQL.";
            exit(0);
        }
    }
    else 
    {
        showLoginDialog();
    }

    initQuery();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePlot);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTableData);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateChart);
    timer->start(60000);

}

void MainWindow::showLoginDialog()
{
   login = new Login(this);
   login->setWindowTitle("Login in database");

   bool declinedConnection = true; 
   while (declinedConnection)
   {
        if(login->exec() == QDialog::Accepted)
        {
            dotenv::init(ENV_PATH);
            declinedConnection = initConnection();
        }
        else 
        {
            exit(0);
        }
   }
}

bool MainWindow::initConnection()
{
    login->copyValues(ip, port, user, password, dbName);
    db.setDatabaseName(dbName);
    db.setHostName(ip);
    db.setPort(port.toInt());

    if(!db.open(user, password))
    {
        QMessageBox::critical(this, "Error", "Invalid input data");
        return true;
    }
    return false;
}

bool MainWindow::initConnection(QString ipTest, QString portTest, QString userTest, 
                                QString passwordTest, QString dbNameTest)
{
    db.setDatabaseName(dbNameTest);
    db.setHostName(ipTest);
    db.setPort(portTest.toInt());
    
    if(!db.open(userTest, passwordTest))
    {
        return true;
    }
    return false;
}

void MainWindow::initQuery()
{
    query = new QSqlQuery(this->db);
}

void MainWindow::updatePlot()
{
    setPlottingData();
    plotNetworkUsage();
}

void MainWindow::execQuery(QString passedQuery)
{
    query->exec(passedQuery);
}

void MainWindow::setPlottingData()
{
    execQuery("SELECT network_usage FROM employee_data ORDER BY id DESC LIMIT " + QString::number(gatheredSize));
    setPlotGathered();

    execQuery("SELECT unnest(prediction) FROM network_predictions ORDER BY id DESC LIMIT 5;");
    setPlotPredicted();
} 

void MainWindow::setPlotGathered()
{
    if (gatheredSize != query->size())
    {
        return;
    }

    record = query->record();

    usage.clear();
    time.clear();
    
    while (query->next()) {
        qreal networkUsage = query->value(0).toFloat();
        usage.push_back(networkUsage);
    }

    std::reverse(usage.begin(), usage.end());

    for (size_t i = 0; i < this->usage.size(); ++i)
    {
        time.push_back(static_cast<qreal>(i));  
    }
}

void MainWindow::setPlotPredicted()
{
    record = query->record();

    usagePred.clear();
    timePred.clear();

    usagePred.push_back(usage[usage.size() - 1]);
    while (query->next()) {
        qreal networkUsage = query->value(0).toFloat();
        usagePred.push_back(networkUsage);
    }

    for (size_t i = usage.size() - 1; i < usage.size() + this->usagePred.size() - 1; ++i)
    {
        timePred.push_back(static_cast<qreal>(i));  
    }

}

void MainWindow::plotNetworkUsage()
{
    if (usage.empty())
    {
        return;
    }

    ui->plotWidget->clearGraphs();

    configurePlot();
    plotNetworkGathered();
    plotNetworkPredicted();

    ui->plotWidget->replot();
}

void MainWindow::configurePlot()
{
    ui->plotWidget->legend->setVisible(true);

    auto maxUsage = std::max_element(usage.begin(), usage.end())[0];
    auto maxTime = time.size() + timePred.size();
    ui->plotWidget->xAxis->setRange(0, maxTime);
    ui->plotWidget->yAxis->setRange(0, maxUsage);

    ui->plotWidget->xAxis->setLabel("Data usage every minute");
    ui->plotWidget->yAxis->setLabel("Network usage");  
}

void MainWindow::plotNetworkGathered()
{
    ui->plotWidget->addGraph();
    ui->plotWidget->graph(0)->setName("Gathered data");
    ui->plotWidget->graph(0)->setData(time, usage);
}

void MainWindow::plotNetworkPredicted()
{
    ui->plotWidget->addGraph();

    QPen bluePen;
    bluePen.setColor(QColor(Qt::red));
    ui->plotWidget->graph(1)->setPen(bluePen);

    ui->plotWidget->graph(1)->setName("Predicted network traffic");
    ui->plotWidget->graph(1)->setData(timePred, usagePred);
}

void MainWindow::updateTableData()
{
    execQuery("SELECT username, violations_amount, name AS pred_action FROM employee AS e INNER JOIN actions_predictions AS a ON e.id = a.user_id INNER JOIN action_patterns AS p ON predicted_action = p.id ORDER BY violations_amount DESC;");
    setTableData();
}

void MainWindow::setTableData()
{
    qStandardItemModel = new QStandardItemModel;
    qStandardItemModel->setHorizontalHeaderLabels(QStringList()<< "User name" << "Violations amount" << "Next possible action");

    QSqlRecord rec = query->record();
    QString username;
    QString violationsAmount;
    QString predictedAction;

    int i{};
    while (query->next())
    {
        username = query->value(0).toString();
        violationsAmount = query->value(1).toString();
        predictedAction = query->value(2).toString();

        auto itemCol1 = new QStandardItem(username);
        auto itemCol2 = new QStandardItem(violationsAmount);
        auto itemCol3 = new QStandardItem(predictedAction);

        qStandardItemModel->appendRow(QList<QStandardItem*>()
                                        << itemCol1 
                                        << itemCol2
                                        << itemCol3);
        
        ui->tableView->setColumnWidth(i, 150 * (i + 1));
        ++i;
    }
    ui->tableView->setColumnWidth(2, 300);
    ui->tableView->setModel(qStandardItemModel);
}

void MainWindow::updateChart()
{
    execQuery("SELECT name, amount FROM (SELECT u.tag AS tag, count(*) AS amount FROM employee_data t CROSS JOIN unnest(t.actions) AS u(tag) GROUP BY u.tag ORDER BY count(*) DESC LIMIT (SELECT COUNT(*) FROM action_patterns)) AS action_count INNER JOIN action_patterns ap ON ap.id = action_count.tag;");
    setChartData();
    plotChart();
}

void MainWindow::setChartData()
{
    QSqlRecord rec = query->record();
    QString name;
    qreal percentage;

    labels.clear();
    percentages.clear();
    while (query->next())
    {
        name = query->value(0).toString();
        percentage = query->value(1).toDouble();
        
        labels.push_back(name);
        percentages.push_back(percentage);
    }
}

void MainWindow::plotChart()
{
    auto series = new QPieSeries;
    for (auto i = 0; i < labels.size(); ++i)
    {
        series->append(labels[i], percentages[i]);
    }

    auto chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("Users actions");
    chart->setVisible(true);

    ui->chartView->setChart(chart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

