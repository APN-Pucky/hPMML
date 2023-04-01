
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include <iostream>
#include <vector>
#include <unordered_map>

#include "hPMML.h"

int main()
{
    std::string linear_regression_filepath = "../benchmark/data/model/IrisMultinomReg.xml";
    std::string decision_tree_filepath = "../benchmark/data/model/IrisTree.xml";
    std::vector<hpmml::Model> models;
    std::vector<hpmml::Prediction> predictions;
    std::unordered_map<std::string, std::string> sample = {
        {"sepal_length", "6.6"},
        {"sepal_width", "2.9"},
        {"petal_length", "4.6"},
        {"petal_width", "1.3"}};

    models.push_back(hpmml::Model(linear_regression_filepath));
    models.push_back(hpmml::Model(decision_tree_filepath));

    for (const auto &model : models)
        predictions.push_back(model.score(sample));

    for (const auto &prediction : predictions)
        std::cout << prediction.as_string() << std::endl;

    return 0;
}
