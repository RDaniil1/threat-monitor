#ifndef AGENTDATABASE_HPP
#define AGENTDATABASE_HPP

#include "fmt/core.h"
#include "fmt/ranges.h"

#include <pqxx/pqxx>

/**
* @brief Creates queries for working with user data.
*/
class AgentDatabase
{
    public:
        /**
        * @brief Adds user data to database from current host.
        *
        * @param nonTr non-transaction object for query execution
        * @param userId ID of user that needs to be added
        * @param encodedActions encoded actions of user, gathered using journalctl
        * @param networkUsage calculated network usage from rx/tx bytes
        */
        void addUserData(pqxx::nontransaction& nonTr, size_t userId, std::vector<size_t>& encodedActions, float networkUsage);
        
        /**
        * @brief Adds new user from current host.
        *
        * @param nonTr non-transaction object for query execution
        * @param username current username of active user
        * @param machineId current machine-id from this host
        */
        void addNewUser(pqxx::nontransaction& nonTr, std::string username, std::string machineId);
        
        /**
        * @brief Get action patterns from database for searching using journalctl.
        *
        * @param res object used for getting result from query
        * @return vector of action patterns
        */
        std::vector<std::string> getActionPatterns(const pqxx::result& res);
};

#endif // AGENTDATABASE_HPP