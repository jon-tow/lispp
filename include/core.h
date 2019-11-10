#ifndef CORE_H
#define CORE_H

#include <cmath>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "exception.h"
#include "type.h"

namespace core {

using CoreFunction =
    std::function<type::LisppObject(std::vector<type::LisppObject>)>;

std::unordered_map<std::string, CoreFunction> build_core();

// Arithmetic
type::LisppObject add(std::vector<type::LisppObject> args);
type::LisppObject sub(std::vector<type::LisppObject> args);
type::LisppObject mul(std::vector<type::LisppObject> args);
type::LisppObject div(std::vector<type::LisppObject> args);

// List Processing
type::LisppObject list(std::vector<type::LisppObject> args);
type::LisppObject is_list(std::vector<type::LisppObject> args);
type::LisppObject is_empty(std::vector<type::LisppObject> args);
type::LisppObject count(std::vector<type::LisppObject> args);
type::LisppObject first(std::vector<type::LisppObject> args);
type::LisppObject rest(std::vector<type::LisppObject> args);

// Logic
type::LisppObject
_not(std::vector<type::LisppObject> args); // `_not` to avoid C++ keyword clash
type::LisppObject equal(std::vector<type::LisppObject> args);
type::LisppObject less(std::vector<type::LisppObject> args);
type::LisppObject less_eq(std::vector<type::LisppObject> args);
type::LisppObject greater(std::vector<type::LisppObject> args);
type::LisppObject greater_eq(std::vector<type::LisppObject> args);

} // namespace core

#endif // CORE_H
