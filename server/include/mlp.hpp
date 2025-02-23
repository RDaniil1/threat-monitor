#ifndef MLP_HPP
#define MLP_HPP

#define MLPACK_ENABLE_ANN_SERIALIZATION
#include "mlpack/core.hpp"
#include "mlpack/methods/ann/layer/layer.hpp"
#include "mlpack/methods/ann/loss_functions/mean_squared_error.hpp"
#include "mlpack/methods/ann/ffn.hpp"

#include <filesystem>

#include "config.hpp"

using namespace mlpack;

class Mlp
{
    private:
        arma::mat data;

        arma::mat testData;
        arma::mat testLabel;

        arma::mat trainData;
        arma::mat trainLabel;

        arma::mat labels;

        arma::mat predictions;

        arma::mat trainPredictions;
        arma::mat testPredictions;

        size_t nextAction;

        FFN<MeanSquaredError, RandomInitialization> model;

        void prepareData();
        void setLayers();
        void setModelData();

        void printError(std::string errorMode, const arma::mat& predictedData, const arma::mat& expectedData);

        void predict(arma::mat& inputData, arma::mat& outputData);

    public:
        Mlp(size_t gatheredAmount);

        void setData(const arma::mat& data);
        arma::mat getData();

        void exec();

        void predict(arma::mat inputData);

        arma::mat getPredictions();

        ~Mlp() = default;
};

#endif