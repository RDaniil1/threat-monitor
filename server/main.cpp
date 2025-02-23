#include <csignal>
#include <thread>
#include <chrono>
#include <csetjmp>

#include <dotenv.h>

#include "arima.hpp"
#include "database.hpp"
#include "mlp.hpp"


std::jmp_buf exitHandler;

[[noreturn]] void handleExit (sig_atomic_t) {
    std::longjmp(exitHandler, true);
}

int main(int argc, char* argv[])
{
    std::signal(SIGINT, handleExit);
    std::signal(SIGTERM, handleExit);
    close(STDOUT_FILENO);

    if (setjmp(exitHandler))
    {
        return 0;
    }

    dotenv::init(ENV_PATH);
    std::string connectionInfo;
    if (DEVELOPER_MODE) 
    {
        connectionInfo = fmt::format("host={} port={} user={} password={} dbname={}", 
                                    std::getenv("host"), std::getenv("port"), std::getenv("user"), 
                                    std::getenv("password"), std::getenv("dbname"));
    }
    else 
    {
        std::string password;
        password = getpass("Enter password: ");
        connectionInfo = fmt::format("host={} port={} user={} password={} dbname={}", 
                                    argv[1], argv[2], argv[3], password, argv[4]);
    }

    pqxx::connection conn(connectionInfo);
    pqxx::nontransaction nonTr(conn);

    Database db(nonTr);

    Mlp mlp(1000);
    
    if (bool modelNotTrained = !std::filesystem::exists(MODEL_PATH))
    {
        db.retrieveTrainingEmployeeActions(200);
        mlp.setData(db.getTrainingEmployeeActions());
        mlp.exec();
    }

    while (true) 
    {
        db.retrieveAgentsNetworkUsage(50);
        auto networkUsage = db.getNetworkUsage();
        Arima arima(5, networkUsage);
        arima.exec();
        auto predictedUsage = arima.getPredicted();
        db.sendPredictedNetworkUsage(predictedUsage);

        std::vector<int> employeesId = db.getEmployeesId();
        for (auto i : employeesId)
        {
            db.setEmployeeId(i);
            db.retrieveViolationsAmount();
            db.retrieveEmployeeActions();

            mlp.predict(db.getEmployeeActions());

            arma::mat pred = mlp.getPredictions();
            size_t nextAction = static_cast<size_t>(pred[0]);
            db.sendPredictedAction(nextAction);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(60000));
    }
    
    return 0;
}