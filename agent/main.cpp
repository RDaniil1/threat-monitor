#ifndef MAIN_CPP
#define MAIN_CPP

#include <csignal>
#include <unistd.h>
#include <csetjmp>

#include <dotenv.h>

#include "conversion.hpp"
#include "agentDatabase.hpp"
#include "parse.hpp"
#include "config.hpp"


std::jmp_buf exitHandler;

[[noreturn]] void handleExit (sig_atomic_t){
    std::longjmp(exitHandler, true);
}

int main(int argc, char** argv)
{
    std::signal(SIGINT, handleExit);
    std::signal(SIGTERM, handleExit);
    std::cout.setstate(std::ios_base::failbit);



    dotenv::init(ENV_PATH);
    std::string connectionInfo;
    std::string networkInterface;
    if (DEVELOPER_MODE)
    {
        
        connectionInfo = fmt::format("host={} port={} user={} password={} dbname={}", 
                                    std::getenv("host"), std::getenv("port"), std::getenv("user"), 
                                    std::getenv("password"), std::getenv("dbname"));
        networkInterface = std::getenv("interface");
    }
    else 
    {
        std::string password;
        password = getpass("Enter password: ");
        connectionInfo = fmt::format("host={} port={} user={} password={} dbname={}", 
                                    argv[1], argv[2], argv[3], password, argv[4]);
        networkInterface = argv[5];
    }

    pqxx::connection conn(connectionInfo);
    pqxx::nontransaction nonTr(conn);

    if (setjmp(exitHandler))
    {
        conn.close();
        return 0;
    }

    Conversion conversion;
    Parse parse;

    std::string machineId = conversion.sha256(parse.getMachineId());
    std::string employeeCheck = fmt::format("SELECT * FROM employee WHERE machine_id = '{}';", 
                                             machineId);

    AgentDatabase agentDatabase;
    if (nonTr.exec(employeeCheck).empty())
    {
        std::string username = getlogin();
        agentDatabase.addNewUser(nonTr, username, machineId);
    }

    std::string queryUserId = fmt::format("SELECT id FROM employee WHERE machine_id = '{}';", machineId);
    pqxx::result resUserId = nonTr.exec(queryUserId);

    pqxx::result resActionPatterns = nonTr.exec("SELECT * FROM action_patterns ORDER BY id;");
    std::vector<std::string> actionPatterns = agentDatabase.getActionPatterns(resActionPatterns);
    size_t userId = resUserId[0][0].as<size_t>();

    while (true)
    {
        std::vector<std::string> currentActions = parse.getCurrentActions(actionPatterns);
        std::vector<size_t> encodedActions = conversion.getEncodedActions(actionPatterns, currentActions);
        
        float networkUsage = parse.getNetworkUsage(networkInterface);
        
        agentDatabase.addUserData(nonTr, userId, encodedActions, networkUsage);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }

    
    conn.close();
    return 0;
}

#endif