#ifndef ARIMA_HPP
#define ARIMA_HPP

#include <iostream>
#include <algorithm>
#include <random>

#include <pqxx/pqxx>
#include "ctsa.h"

#include "database.hpp"

typedef std::vector<double> doubleVec;

/**
* @brief Create and use ARIMA model for forecasting network load.
*/
class Arima
{
    public:
        Arima(size_t L, arma::rowvec networkUsage);
        Arima() = delete;

        /**
        * @brief Train and predict ARIMA model.
        */
        void exec();
        /**
        * @brief Get mean squared error value, based on trained model.
        *
        * @return mean squared error for trained model
        */
        doubleVec getAmse();
        /**
        * @brief Get predicted network traffic load values, based on trained model.
        *
        * @return network traffic load values from trained model
        */
        doubleVec getPredicted();

        ~Arima()
        {
            auto_arima_free(autoArimaObject);
        }
    private:
        auto_arima_object autoArimaObject; /**< ARIMA object, that calulates values for model training */

        doubleVec trainData; /**< user data, used for model training on network load */

        size_t L; /**< amount of captured user data, that will be used in training */

        doubleVec pred; /**< predicted network load values */
        doubleVec amse; /**< predicted network load values */

        /**
        * @brief Train ARIMA model, based on user data.
        */
        void train();
        /**
        * @brief Predict next network traffic load values, based on ARIMA model.
        */
        void predict();
};

#endif // ARIMA_HPP