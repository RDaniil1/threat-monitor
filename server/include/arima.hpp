#ifndef ARIMA_HPP
#define ARIMA_HPP

#include <iostream>
#include <algorithm>
#include <random>

#include "ctsa.h"
#include <pqxx/pqxx>

#include "database.hpp"

typedef std::vector<double> doubleVec;

class Arima
{
    private:
        auto_arima_object autoArimaObject;

        doubleVec trainData;

        size_t L;

        doubleVec pred;
        doubleVec amse;

        void train();
        void predict();
    public:
        Arima(size_t L, arma::rowvec networkUsage);

        void exec();
        
        doubleVec getAmse();
        doubleVec getPredicted();

        // ~Arima()
        // {
        //     auto_arima_free(autoArimaObject);
        // }
};

#endif