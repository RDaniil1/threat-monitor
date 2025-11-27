#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sstream>

#include <pqxx/pqxx>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "mlpack/core.hpp"

/**
* @brief Get and set data from database for ARIMA and MLP models.
*/
class Database
{
    public:
        explicit Database(pqxx::nontransaction& nonTr);
        Database() = delete;
        /**
        * @brief Get users network load.
        *
        * @param dataLimit limit of data that needs to be retrieved from database
        */
        void retrieveAgentsNetworkUsage(size_t dataLimit);
        /**
        * @brief Get users actions for training MLP model.
        *
        * @param dataLimit limit of data that needs to be retrieved from database
        */
        void retrieveTrainingEmployeeActions(size_t dataLimit);
        /**
        * @brief Set current user's ID.
        *
        * @param employeeId current user's ID
        */        
        void setEmployeeId(size_t employeeId);
        /**
        * @brief Get last user's actions.
        */
        void retrieveEmployeeActions();
        /**
        * @brief Get last user's actions amount.
        */
        void retrieveViolationsAmount();
        /**
        * @brief Get user's network load.
        *
        * @return user's network load
        */
        arma::rowvec getNetworkUsage() const;
        /**
        * @brief Get users actions that will be used in training.
        *
        * @return captured users actions
        */
        arma::mat getTrainingEmployeeActions() const;
        /**
        * @brief Get user's actions.
        *
        * @return captured user's actions
        */
        arma::rowvec getEmployeeActions() const;
        /**
        * @brief Get user's current ID.
        *
        * @return user's current ID
        */
        int getEmployeeId() const;
        /**
        * @brief Get users current ID's.
        *
        * @return users current ID's
        */
        std::vector<int> getEmployeesId() const;
        /**
        * @brief Send predicted network load to database.
        *
        * @param predNetworkUsage predicted network load from ARIMA model
        */ 
        void sendPredictedNetworkUsage(std::vector<double> predNetworkUsage);
        /**
        * @brief Send predicted next user's action.
        *
        * @param nextAction predicted next user's action
        */ 
        void sendPredictedAction(size_t nextAction);
        ~Database() = default;
    private:
        pqxx::nontransaction* nonTr; /**< object used for querying database as non-transaction */

        int employeeAmount; /**< users amount for gathering database data */

        arma::rowvec networkUsage; /**< user's network load */

        pqxx::result output; /**< output from database on each query */
        arma::mat trainingEmployeeActions; /**< captured user data, that will be used in model training */

        int employeeId; /**< current user ID */
        std::vector<int> employeesId; /**< user's ID for traversing on MLP model */
        int employeeViolationsAmount;  /**< current user violations amount */
        arma::mat employeeActions; /**< current user violations */
        /**
        * @brief Parse user actions from database.
        *
        * @param actionsIndex actions index row in database
        * @return rowvec of encoded actions 
        */
        arma::rowvec parseEmployeeActions(size_t actionsIndex) const; 
        /**
        * @brief Update database with predicted user action.
        *
        * @param predEmployeeAction predicted user action
        */
        void updatePredictedEmployeeValues(const size_t& predEmployeeAction);
        /**
        * @brief Set ID for each user from database.
        */
        void setEmployeesId();
};

#endif // DATABASE_HPP