#include "agentDatabase.hpp"

void AgentDatabase::addUserData(pqxx::nontransaction& nonTr, size_t userId, std::vector<size_t>& encodedActions, float networkUsage)
{
    std::string query  = fmt::format("INSERT INTO employee_data (user_id, actions, network_usage) VALUES ({}, ARRAY [{}], {});",
                                      userId, fmt::join(encodedActions, ", "), networkUsage);
    nonTr.exec(query);
}

void AgentDatabase::addNewUser(pqxx::nontransaction& nonTr, std::string username, std::string machineId)
{
    std::string query = fmt::format("INSERT INTO employee (username, machine_id) VALUES ('{}', '{}');",
                                      username, machineId);
    nonTr.exec(query);
}

std::vector<std::string> AgentDatabase::getActionPatterns(const pqxx::result& res)
{
    unsigned long const numRows = res.size();
    std::vector<std::string> actionPatterns;
    actionPatterns.reserve(numRows);

    std::string pattern;
    for (size_t i{}; i < numRows; ++i)
    {
        pqxx::row const row = res[i];
        pqxx::field const field = row[2];
        pattern = field.as<std::string>();
        actionPatterns.push_back(pattern);
    }
    return actionPatterns;
}