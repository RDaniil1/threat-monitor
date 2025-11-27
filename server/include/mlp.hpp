#ifndef MLP_HPP
#define MLP_HPP

#include <filesystem>

#define MLPACK_ENABLE_ANN_SERIALIZATION
#include "mlpack/core.hpp"
#include "mlpack/methods/ann/layer/layer.hpp"
#include "mlpack/methods/ann/loss_functions/mean_squared_error.hpp"
#include "mlpack/methods/ann/ffn.hpp"

#include "config.hpp"

using namespace mlpack;

/**
* @brief Create and use MLP model for predicting future users action.
*/
class Mlp
{
    private:
        arma::mat data; /**< action values used for training MLP model */
        arma::mat labels; /**< target action values used for training MLP model */

        arma::mat testData; /**< test action values used to evaluate MLP model */
        arma::mat testLabel; /**< test action values used to evaluate MLP model */

        arma::mat trainData; /**< action values used for training MLP model */
        arma::mat trainLabel; /**< target action values used for training MLP model */

        arma::mat predictions; /**< MLP model predicted action values */

        arma::mat trainPredictions; /**< MLP model train predicted action values */
        arma::mat testPredictions; /**< MLP model test predicted action values */

        size_t nextAction; /**< current next user's action */

        FFN<MeanSquaredError, RandomInitialization> model; /**< MLP model used for prediction user's actions */
        /**
        * @brief Get labels from dataset and split in training and test data.
        */
        void prepareData();
        /**
        * @brief Set MLP model layers.
        */
        void setLayers();
        /**
        * @brief Train MLP model.
        */
        void train();
        /**
        * @brief Save MLP model to host.
        */
        void save();
        /**
        * @brief Print accuracy on training model.
        *
        * @param errorMode mode, used for estimation; could be train or test
        * @param predictedData predicted actions from MLP model
        * @param expectedData expected labels from input
        */
        void printAccuracy(std::string errorMode, const arma::mat& predictedData, const arma::mat& expectedData);
        /**
        * @brief Predict user's action.
        *
        * @param inputData input user's data for MLP model
        * @param outputData output user's data for MLP model 
        */
        void predict(arma::mat& inputData, arma::mat& outputData);
    public:
        Mlp(size_t gatheredAmount);
        /**
        * @brief Set data for MLP model.
        *
        * @param data input user's data for MLP model
        */
        void setData(const arma::mat& data);
        /**
        * @brief Get current data.
        *
        * @return current MLP model data
        */
        arma::mat getData();
        /**
        * @brief Create MLP model and save if not exist.
        */
        void exec();
        /**
        * @brief Predict MLP model data.
        *
        * @param inputData input user's data for prediction
        */
        void predict(arma::mat inputData);
        /**
        * @brief Get MLP model predictions.
        *
        * @return MLP model predictions
        */
        arma::mat getPredictions();

        ~Mlp() = default;
};

#endif // MLP_HPP