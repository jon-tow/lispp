#ifndef CORE_H
#define CORE_H

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "exception.h"
#include "printer.h"
#include "type.h"

namespace core {

using CoreFunction =
    std::function<type::LisppObject(std::vector<type::LisppObject>)>;

std::unordered_map<std::string, CoreFunction> build_core();

// I/O
type::LisppObject print(std::vector<type::LisppObject> args);

// Arithmetic
type::LisppObject add(std::vector<type::LisppObject> args);
type::LisppObject sub(std::vector<type::LisppObject> args);
type::LisppObject mul(std::vector<type::LisppObject> args);
type::LisppObject div(std::vector<type::LisppObject> args);

// List Processing
type::LisppObject list(std::vector<type::LisppObject> args);
type::LisppObject is_empty(std::vector<type::LisppObject> args);
type::LisppObject count(std::vector<type::LisppObject> args);
type::LisppObject first(std::vector<type::LisppObject> args);
type::LisppObject rest(std::vector<type::LisppObject> args);

// Type Predicates
type::LisppObject is_list(std::vector<type::LisppObject> args);
type::LisppObject is_nil(std::vector<type::LisppObject> args);
type::LisppObject is_true(std::vector<type::LisppObject> args);
type::LisppObject is_false(std::vector<type::LisppObject> args);
type::LisppObject is_symbol(std::vector<type::LisppObject> args);
type::LisppObject is_number(std::vector<type::LisppObject> args);

// Logic
// Note: `_` prefix to avoid C++ keyword clash.
type::LisppObject _not(std::vector<type::LisppObject> args);
type::LisppObject _and(std::vector<type::LisppObject> args);
type::LisppObject _or(std::vector<type::LisppObject> args);

// Comparisons
type::LisppObject less(std::vector<type::LisppObject> args);
type::LisppObject less_eq(std::vector<type::LisppObject> args);
type::LisppObject greater(std::vector<type::LisppObject> args);
type::LisppObject greater_eq(std::vector<type::LisppObject> args);
type::LisppObject equal(std::vector<type::LisppObject> args);

} // namespace core

#endif // CORE_H
