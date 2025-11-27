/*********************************************************************
 * @file  arima.cpp
 * 
 * @brief Implementation of the class Arima.
 *********************************************************************/
#include "arima.hpp"

Arima::Arima(size_t L, arma::rowvec networkUsage)
{
    this->L = L;
    trainData = arma::conv_to<doubleVec>::from(networkUsage.row(0));
}

void Arima::exec()
{
    train();
    predict();
}

void Arima::train()
{
    autoArimaObject = auto_arima_init(nullptr, nullptr, NULL, NULL, (int)trainData.size());
    auto_arima_setStationarityParameters(autoArimaObject, "adf", 0.05, "level");
    auto_arima_exec(autoArimaObject, &trainData[0], nullptr);
}

void Arima::predict()
{
    pred.resize(L);
    std::fill(pred.begin(), pred.end(), 0);
    amse.resize(L);
    std::fill(amse.begin(), amse.end(), 0);

    auto_arima_predict(autoArimaObject, &trainData[0], NULL, L, NULL, &pred[0], &amse[0]);

    double errorSum =  std::accumulate(amse.begin(), amse.end(), 0.0); ;
    double mseResult =  errorSum / static_cast<double>(amse.size());
    std::cout << "Mean squared error ARIMA: " << mseResult << '\n';

    // auto_arima_summary(autoArimaObject);

    auto_arima_free(autoArimaObject);
}

doubleVec Arima::getAmse()
{
    if (amse.empty())
    {
        return {};
    }
    return amse;
}

doubleVec Arima::getPredicted()
{    
    if (pred.empty())
    {
        return {};
    }
    return pred;
}


