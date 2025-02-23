#ifndef AGENTDATABASE_HPP
#define AGENTDATABASE_HPP

#include "fmt/core.h"
#include "fmt/ranges.h"

#include <pqxx/pqxx>

class AgentDatabase
{
    public:
        void addUserData(pqxx::nontransaction& nonTr, size_t userId, std::vector<size_t>& encodedActions, float networkUsage);
        void addNewUser(pqxx::nontransaction& nonTr, std::string username, std::string machineId);

        std::vector<std::string> getActionPatterns(const pqxx::result& res);
};


#endif