/*********************************************************************
 * @file  database.cpp
 * 
 * @brief Implementation of the class Database.
 *********************************************************************/
#include "database.hpp"

Database::Database(pqxx::nontransaction& nonTr) 
{
    this->employeeAmount = -1;
    this->employeeId = -1;
    this->employeeViolationsAmount = -1;
    
    this->nonTr = &nonTr;
    
    setEmployeesId();
}

void Database::setEmployeesId()
{
    std::string query = "SELECT id FROM employee ORDER BY id;";
    pqxx::result employeeIdRes = nonTr->exec(query);

    for (int i{}; i < employeeIdRes.size(); ++i)
    {
        this->employeesId.push_back(employeeIdRes[i][0].as<int>());
    }
}

void Database::retrieveAgentsNetworkUsage(size_t dataLimit)
{

    std::string query = fmt::format("SELECT COALESCE(current_network_load, 0) FROM gathered_network({});", dataLimit);
    pqxx::result employeeData = nonTr->exec(query);

    if (employeeData.size() != dataLimit)
    {
        return;
    }

    networkUsage.set_size(dataLimit);
    size_t j = 0; 
    for (int i{employeeData.size() - 1}; i >= 0; --i)
    {
        auto networkAmount = employeeData[i][0].as<float>();
        networkUsage.col(j) = networkAmount;
        ++j;
    }
}

void Database::retrieveTrainingEmployeeActions(size_t dataLimit)
{
    std::string query = fmt::format("SELECT actions FROM employee_data ORDER BY id DESC LIMIT {};", dataLimit);
    output = nonTr->exec(query);

    if (bool notEnoughData = output.size() != dataLimit)
    {
        return;
    }

    trainingEmployeeActions.set_size(dataLimit, 10);
    for (size_t i{}; i < trainingEmployeeActions.n_rows; ++i)
    {
        trainingEmployeeActions.row(i) = parseEmployeeActions(i);
    }
}

arma::rowvec Database::parseEmployeeActions(size_t actionsIndex) const
{
    arma::rowvec parsedActions(10);
    std::string choosedActions = output[actionsIndex][0].c_str();
    choosedActions.erase(choosedActions.begin());
    choosedActions.erase(choosedActions.end() - 1);

    std::string token;
    std::stringstream ss(choosedActions);
    size_t i{};
    while (getline(ss, token, ',')){
        parsedActions.col(i) = std::stod(token);
        i++;
    }

    return parsedActions;
}

void Database::setEmployeeId(size_t id)
{
    this->employeeId = (int)id;
}

void Database::retrieveEmployeeActions()
{
    std::string query = fmt::format("SELECT actions FROM employee_data WHERE user_id = {} ORDER BY id DESC LIMIT 1 ;", employeeId);
    output = nonTr->exec(query);
    employeeActions = parseEmployeeActions(0);
}

void Database::retrieveViolationsAmount()
{
    std::string query = fmt::format("SELECT SUM(violations_amount) FROM (SELECT (SELECT SUM(case b when '0' then 0 else 1 end) FROM unnest(actions) AS dt(b)) AS violations_amount FROM employee_data WHERE user_id = {} ORDER BY id DESC) AS limited_violations_amount;", employeeId);
    output = nonTr->exec(query);
    employeeViolationsAmount = output[0][0].as<int>();
}

arma::rowvec Database::getNetworkUsage() const
{
    if (networkUsage.empty())
    {
        arma::rowvec empty(1, 1, arma::fill::zeros);
        return empty;
    }

    return networkUsage;
}

arma::mat Database::getTrainingEmployeeActions() const
{
    if (trainingEmployeeActions.empty())
    {
        arma::mat empty(1, 1, arma::fill::zeros);
        return empty;
    }

    return trainingEmployeeActions;
}

arma::rowvec Database::getEmployeeActions() const
{
    if (employeeActions.empty())
    {
        arma::mat empty(1, 1, arma::fill::zeros);
        return empty;
    }

    return employeeActions;
}

int Database::getEmployeeId() const
{
    return employeeId;
}

std::vector<int> Database::getEmployeesId() const
{
    if (this->employeesId.empty())
    {
        return {0};
    }
    return this->employeesId;
}

void Database::sendPredictedNetworkUsage(std::vector<double> predNetworkUsage)
{
    std::string query = fmt::format("INSERT INTO network_predictions (prediction) VALUES (ARRAY [{}]);", fmt::join(predNetworkUsage, ", "));
    nonTr->exec(query);
}

void Database::sendPredictedAction(size_t nextAction)
{
    std::string query = fmt::format("SELECT * FROM actions_predictions WHERE user_id = {}", employeeId);
    
    if (pqxx::result res = nonTr->exec(query); !res.empty())
    {
        updatePredictedEmployeeValues(nextAction);
        return;
    } 

    query = fmt::format("INSERT INTO actions_predictions (user_id, violations_amount, predicted_action) VALUES ({}, {}, {});", employeeId, employeeViolationsAmount, nextAction);
    nonTr->exec(query);
}

void Database::updatePredictedEmployeeValues(const size_t& predEmployeeAction)
{
    std::string query = fmt::format("UPDATE actions_predictions SET violations_amount = {}, predicted_action = {} WHERE user_id = {};", employeeViolationsAmount, predEmployeeAction, employeeId);
    nonTr->exec(query);
}