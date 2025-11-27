/*********************************************************************
 * @file  mlp.cpp
 * 
 * @brief Implementation of the class Mlp.
 *********************************************************************/
#include "mlp.hpp"

Mlp::Mlp(size_t gatheredAmount)
{
    data.set_size(gatheredAmount, 10);

    if (std::filesystem::exists(MODEL_PATH))
    {
        data::Load(MODEL_PATH, "model", model, false);
    }
}

void Mlp::setData(const arma::mat& data)
{
    this->data = data;
}

arma::mat Mlp::getData()
{
    if (data.empty())
    {
        arma::mat empty(1, 1, arma::fill::zeros);
        return empty;
    }

    return data;
}

void Mlp::exec()
{
    prepareData();
    setLayers();
    train();
    save();

    predict(trainData, trainPredictions);
    predict(testData, testPredictions);

    printAccuracy("Train", trainPredictions, trainLabel);
    printAccuracy("Test", testPredictions, testLabel);
}

void Mlp::prepareData()
{
    data = data.t();

    labels = data.row(data.n_rows - 1); 
    data.shed_row(data.n_rows - 1);

    mlpack::data::Split(data, labels, trainData, testData, trainLabel, testLabel, 0.25);
}

void Mlp::setLayers()
{
    model.Add<Linear>(9); 
    model.Add<ReLU>(); 
    model.Add<Linear>(3); 
    model.Add<ReLU>(); 
    model.Add<Linear>(1); 
    model.Add<ReLU>();
}

void Mlp::train()
{
    model.Train(trainData, trainLabel);
}

void Mlp::save()
{
    data::Save(MODEL_PATH, "model", model, false);
}

void Mlp::printAccuracy(std::string errorMode, const arma::mat& predictedData, const arma::mat& expectedData)
{
    arma::umat _predictedData = arma::conv_to<arma::umat>::from(predictedData);
    arma::umat _expectedData = arma::conv_to<arma::umat>::from(expectedData);

    const double trainError = 
            arma::accu(_predictedData == _expectedData) * 1.0 / _expectedData.n_elem;
    std::cout << errorMode << " accuracy: " << trainError << '\n';
}

void Mlp::predict(arma::mat& inputData, arma::mat& outputData)
{
    model.Predict(inputData, outputData);
}

void Mlp::predict(arma::mat inputData)
{
    inputData.shed_col(0);
    inputData = inputData.t();
    model.Predict(inputData, predictions);
}

arma::mat Mlp::getPredictions()
{
    if (predictions.empty())
    {
        arma::mat empty(1, 1, arma::fill::zeros);
        return empty;
    }
    return predictions;
}
