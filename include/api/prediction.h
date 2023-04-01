
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include "../cPMML.h"
#include "../core/internal_score.h"

namespace cpmml {
inline Prediction::Prediction(const std::shared_ptr<InternalScore>& score) : score(score) {}

inline std::string Prediction::as_string() const { return score->score; }

inline double Prediction::as_double() const { return score->double_score; }

inline std::unordered_map<std::string, double> Prediction::distribution() const { return score->probabilities; }
inline std::unordered_map<std::string, double> Prediction::num_outputs() const { return score->num_outputs; }
inline std::unordered_map<std::string, std::string> Prediction::str_outputs() const { return score->str_outputs; }
}  // namespace cpmml
