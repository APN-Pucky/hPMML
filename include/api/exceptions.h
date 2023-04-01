
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include "../hPMML.h"

namespace hpmml {
inline Exception::Exception(const std::string &message) : message(message) {}

inline const char *Exception::what() const noexcept { return (message).c_str(); }

inline MissingValueException::MissingValueException(const std::string &message) : Exception(message) {}

inline InvalidValueException::InvalidValueException(const std::string &message) : Exception(message) {}

inline MathException::MathException(const std::string &message) : Exception(message) {}

inline ParsingException::ParsingException(const std::string &message) : Exception(message) {}
}  // namespace hpmml
