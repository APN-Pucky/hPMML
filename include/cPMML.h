
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_CPMML_H
#define CPMML_CPMML_H


#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

namespace cpmml {
/**
 * cPMML version
 * Stores which version of the cPMML library is used.
 */
extern const std::string version;

/**
 * @class Exception
 * @brief Base class for all exceptions generated by cPMML.
 */
class Exception : public std::exception {
 public:
  /**
   * Constructs a new Exception object.
   * @param message explanatory string providing more details about the error.
   */
  explicit Exception(const std::string &message);

  /**
   * @return an explanatory string providing more details about the error.
   */
  virtual const char *what() const noexcept;

 private:
  /**
   * Explanatory string providing more details about the error.
   */
  std::string message;
};

/**
 * @class MissingValueException
 * @brief Reports errors related to the absence of values in user input.
 *
 * For instance a missing feature in a sample to be scored. Notice that,
 * depending on the underlying PMML, some models may tolerate missing values. It
 * is generated during model scoring.
 */
class MissingValueException : public Exception {
 public:
  /**
   * Constructs a new MissingValueException object.
   * @param message explanatory string providing more details about the error.
   */
  explicit MissingValueException(const std::string &message);
};

/**
 * @class InvalidValueException
 * @brief Reports errors related to the presence of wrong values in user input.
 *
 * For instance a categorical feature assuming a value not in the domain or
 * continuous features assuming a value outside certain boundaries. It is
 * generated during input validation or during model scoring.
 * @see Model#validate
 */
class InvalidValueException : public Exception {
 public:
  /**
   * Constructs a new InvalidValueException object.
   * @param message explanatory string providing more details about the error.
   */
  explicit InvalidValueException(const std::string &message);
};

/**
 * @class MathException
 * @brief Reports errors encountered in the computation of a mathematical
 * function. For example when a normalization methods receives wrong values.
 *
 */
class MathException : public Exception {
 public:
  /**
   * Constructs a new MathException object.
   * @param message explanatory string providing more details about the error.
   */
  explicit MathException(const std::string &message);
};

/**
 * @class ParsingException
 * @brief Reports errors related to the parsing of the user input or of the PMML
 * file representing the model.
 *
 * For instance this error could be triggered in case a certain PMML element or
 * attribute, needed for the scoring, is missing.
 *
 * @see Model
 */
class ParsingException : public Exception {
 public:
  /**
   * Constructs a new ParsingException object.
   * @param message explanatory string providing more details about the error.
   */
  explicit ParsingException(const std::string &message);
};
}  // namespace cpmml

class InternalScore;
namespace cpmml {

/**
 * @class Prediction
 * @brief Class representing a prediction.
 *
 * This class has no corresponding PMML element since it contains the output of
 * a PMML model evaluation.
 *
 * It stores the predicted value, already transformed through <a
 * href="http://dmg.org/pmml/v4-4/Targets.html">PMML Target</a>, along with the
 * additional outputs provided by <a
 * href="http://dmg.org/pmml/v4-4/Output.html">PMML Output</a>. In addition, for
 * classification models allowing the calculation of target probabilities, it
 * provides the probability for each target class.
 */
class Prediction {
 public:
  Prediction() = default;

  explicit Prediction(const std::shared_ptr<InternalScore> &score);

  /**
   * @brief It returns the predicted value as a string.
   */
  std::string as_string() const;

  /**
   * @brief It returns the predicted value as a double. In case the predicted
   * value cannot be converted to double, it will return <a
   * href="https://en.cppreference.com/w/cpp/types/numeric_limits/min">std::numeric_limits<double>::min()</a>.
   */
  double as_double() const;

  /**
   * @brief It returns an hash map of string -> double pairs, where the keys are
   * the target categories and the values are the associated probabilities
   * computed by the model.
   *
   *
   * <br><p><b>Examples</b></p>
   * @code{.cpp}
   * cpmml::Model model(model_filepath);
   * std::unordered_map<std::string, std::string> sample = {
   *  {"sepal_length", "6.6"},
   *  {"sepal_width", "2.9"},
   *  {"petal_length", "4.6"},
   *  {"petal_width", "1.3"}
   * };
   *
   * cpmml::Prediction prediction = model.score(sample);
   * for (const auto &probability : prediction.distribution())
   *    std::cout << probability.first << ": " << probability.second <<
   * std::endl;
   *
   * // "Iris-virginica: 0.0810811"
   * // "Iris-setosa: 0"
   * // "Iris-versicolor: 0.918919"
   * @endcode
   */
  std::unordered_map<std::string, double> distribution() const;

  /**
   * @brief It returns an hash map of string -> double pairs, where the keys are
   * the output field names and the values their corresponding value. Among all
   * the output fields available for a model, it contains just numeric output
   * fields.
   *
   *
   * <br><p><b>Examples</b></p>
   * @code{.cpp}
   * cpmml::Model model(model_filepath);
   * std::unordered_map<std::string, std::string> sample = {
   *  {"sepal_length", "6.6"},
   *  {"sepal_width", "2.9"},
   *  {"petal_length", "4.6"},
   *  {"petal_width", "1.3"}
   * };
   *
   * cpmml::Prediction prediction = model.score(sample);
   * for (const auto &output : prediction.num_outputs())
   *    std::cout << output.first << ": " << output.second << std::endl;
   *
   * // "Probability_Iris-setosa: 0"
   * // "Probability_Iris-virginica: 0.0810811"
   * // "Probability_Iris-versicolor: 0.918919"
   * @endcode
   */
  std::unordered_map<std::string, double> num_outputs() const;

  /**
   * @brief It returns an hash map of string -> string pairs, where the keys are
   * the output field names and the values their corresponding value. Among all
   * the output fields available for a model, it contains just categorical
   * output fields.
   */
  std::unordered_map<std::string, std::string> str_outputs() const;

 private:
  std::shared_ptr<InternalScore> score;
};
}  // namespace cpmml

class InternalEvaluator;
namespace cpmml {

/**
 *  @class  Model
 *  @brief  Class representing a PMML model.
 */
class Model {
 public:
  Model() = default;

  explicit Model(const std::string &model_filepath);

  /**
   * @brief Constructs a cpmml::Model instance representing the PMML model
   * stored at *model_filepath*.
   *
   * <p>
   * It triggers the *model load* process: the XML elements are visited and the
   * corresponding internal cPMML objects are built.<br>
   *
   * It is responsibility of the user to ensure the input file is compliant with
   * the <a href="http://dmg.org/pmml/v4-4/pmml-4-4.xsd">PMML XML schema</a>.
   * Creating cpmml::Model instances from incorrect PMML files may result in
   * undefined behavior.<br>
   *
   * cpmml::ParsingException will be thrown in case *model_path* does not
   * exists.<br>
   *
   * File extension is not taken into account.<br></p>
   *
   *
   * @param model_filepath path to the XML file containing the PMML model.
   * @param zipped *(optional)* it allows to specify whether the input file is
   * compressed in zip format. The default value is *false*.
   *
   * @throws cpmml::ParsingException
   * @throws cpmml::InvalidValueException
   * @throws cpmml::MissingValueException
   *
   *
   * <br><p><b>Examples</b></p>
   * @code{.cpp}
   * cpmml::Model model("IrisTree.xml");
   * @endcode
   * @code{.cpp}
   * cpmml::Model model("AuditRandomForest.zip", true);
   * @endcode
   */
  Model(const std::string &model_filepath, const bool zipped);

  /**
   * @brief Validates user input in *sample* against the constraints defined in
   * the <a href="http://dmg.org/pmml/v4-4/DataDictionary.html">PMML
   * DataDictionary</a>.
   *
   * <p>The PMML standard allows to define constraints on every input field of
   * the model in order to determine the validity of the values assumed by the
   * variables. For continuous fields, it is verified they fall in a certain
   * range. For categorical fields, it is checked they assume a value among a
   * list of possible ones. Being PMML strongly typed, also the type of the
   * input variables is checked.<br>
   *
   * In addition, if <a
   * href="http://dmg.org/pmml/v4-4/Transformations.html">PMML
   * TransformationDictionary</a> is present, it is also checked the possibility
   * to compute all transformations needed for the model.<br></p>
   *
   *
   * @param sample hash map where the keys are strings representing feature
   * names and the values are strings representing features values.
   * @return *true* in case of valid input *sample*.  *false* otherwise.
   *
   *
   * <br><p><b>Examples</b></p>
   * @code{.cpp}
   * cpmml::Model model("IrisTree.xml");
   * std::unordered_map<std::string, std::string> sample = {
   *   {"sepal_length", "6.6"},
   *   {"sepal_width", "2.9"},
   *   {"petal_length", "4.6"},
   *   {"petal_width", "1.3"}
   * };
   *
   * if(!model.validate(sample)) std::cout << "Invalid input for model" <<
   * std::endl;
   * @endcode
   */
  bool validate(const std::unordered_map<std::string, std::string> &sample) const;

  /**
   * @brief Scores the model against the user input in *sample*.
   *
   * <p>
   * It triggers the *model scoring* process: the input values are preprocessed,
   * validated, used as input for the PMML model and the resulting prediction is
   * postprocessed.<br></p>
   *
   *
   * @param sample hash map where the keys are strings representing feature
   * names and the values are strings representing features values.
   * @return An instance of cpmml::Prediction
   *
   * @throws cpmml::InvalidValueException
   * @throws cpmml::MissingValueException
   * @throws cpmml::MathException
   *
   *
   * <br><p><b>Examples</b></p>
   * @code{.cpp}
   * cpmml::Model model("IrisTree.xml");
   * std::unordered_map<std::string, std::string> sample = {
   *   {"sepal_length", "6.6"},
   *   {"sepal_width", "2.9"},
   *   {"petal_length", "4.6"},
   *   {"petal_width", "1.3"}
   * };
   *
   * cpmml::Prediction prediction = model.score(sample);
   * @endcode
   */
  Prediction score(const std::unordered_map<std::string, std::string> &sample) const;

  /**
   * @brief Scores the model against the user input in *sample*.
   *
   * <p>
   * It triggers a minimal version of the *model scoring* process: the input
   * values are preprocessed, validated, used as input for the PMML model and
   * the raw resulting prediction is returned.<br>
   *
   * It has been included to easily retrieve the raw prediction value in case of
   * no interest for other model outputs (eg. probabilities, etc.).<br>
   *
   * In addition, returning the raw prediction allows further internal
   * optimization. Thus, cpmml::Model::predict achieves a significant speedup
   * compared to cpmml::Model::score.<br>
   * </p>
   *
   *
   * @param sample hash map where the keys are strings representing feature
   * names and the values are strings representing features values.
   * @return A string containing the raw prediction.
   *
   * @throws cpmml::InvalidValueException
   * @throws cpmml::MissingValueException
   * @throws cpmml::MathException
   *
   *
   * <br><p><b>Examples</b></p>
   * @code{.cpp}
   * cpmml::Model model("IrisTree.xml");
   * std::unordered_map<std::string, std::string> sample = {
   *   {"sepal_length", "6.6"},
   *   {"sepal_width", "2.9"},
   *   {"petal_length", "4.6"},
   *   {"petal_width", "1.3"}
   * };
   *
   * std::cout << model.predict(sample); // "Iris-versicolor"
   * @endcode
   */
  std::string predict(const std::unordered_map<std::string, std::string> &sample) const;

 private:
  std::shared_ptr<InternalEvaluator> evaluator;
};
}  // namespace cpmml

#include "api/exceptions.h"
#include "api/model.h"
#include "api/prediction.h"
#include "api/version.h"
#endif

