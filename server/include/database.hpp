#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sstream>

#include <pqxx/pqxx>
#include "fmt/core.h"
#include "fmt/ranges.h"

#include "mlpack/core.hpp"

class Database
{
    private:
        pqxx::nontransaction* nonTr;

        int employeeAmount = -1;

        arma::rowvec networkUsage;

        pqxx::result output;
        arma::mat trainingEmployeeActions;

        int employeeId = -1;
        std::vector<int> employeesId;
        int employeeViolationsAmount = -1;
        arma::mat employeeActions;

        arma::rowvec parseEmployeeActions(size_t actionsIndex) const;
        void updatePredictedEmployeeValues(const size_t& predEmployeeAction);

        void setEmployeesId();
    public:
        explicit Database(pqxx::nontransaction& nonTr);
        
        void retrieveAgentsNetworkUsage(size_t dataLimit);
        void retrieveTrainingEmployeeActions(size_t dataLimit);
        void setEmployeeId(size_t employeeId);

        void retrieveEmployeeActions();
        void retrieveViolationsAmount();

        arma::rowvec getNetworkUsage() const;
        arma::mat getTrainingEmployeeActions() const;
        arma::rowvec getEmployeeActions() const;
        int getEmployeeId() const;
        std::vector<int> getEmployeesId() const;

        void sendPredictedNetworkUsage(std::vector<double> predNetworkUsage);
        void sendPredictedAction(size_t nextAction);

        ~Database() = default;
};

#endif