#include "arima.hpp"
#include "database.hpp"
#include "mlp.hpp"

#include "dotenv.h"

#include <gtest/gtest.h>

#include <map>


TEST(ArimaTest, EmptyCheck) 
{
    size_t L = 5;
    Arima arima(L, {});

    EXPECT_EQ(arima.getAmse(), (doubleVec){}) << "Amse values should be empty";
    EXPECT_EQ(arima.getPredicted(), (doubleVec){}) << "Predicted values should be empty";
}

TEST(MlpTest, EmptyCheck) 
{
    Mlp mlp(10);
    
    EXPECT_EQ(mlp.getPredictions()[0], 0) << "Predicted values should be empty";
}

TEST(DatabaseTest, EmptyCheck) 
{
    dotenv::init(ENV_PATH);
    std::string connectionInfo = fmt::format("host={} port={} user={} password={} dbname={}", 
                              std::getenv("host"), std::getenv("port"), std::getenv("user"), 
                              std::getenv("password"), std::getenv("dbname"));

    pqxx::connection conn(connectionInfo);
    pqxx::nontransaction nonTr(conn);

    Database db(nonTr);

    EXPECT_EQ(db.getNetworkUsage()[0], 0) << "Network usage should be empty";
    EXPECT_EQ(db.getTrainingEmployeeActions()[0], 0) << "Training employee actions should be empty";
    EXPECT_EQ(db.getEmployeeActions()[0], 0) << "Employee actions should be empty";
    EXPECT_EQ(db.getEmployeeId(), -1) << "EmployeeId should be empty";
}