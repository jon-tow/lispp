#ifndef OPERATORS_H
#define OPERATORS_H

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "exception.h"
#include "printer.h"
#include "type.h"

namespace operators {

// Arithmetic
type::LisppObject add(std::vector<type::LisppObject> args);
type::LisppObject sub(std::vector<type::LisppObject> args);
type::LisppObject mul(std::vector<type::LisppObject> args);
type::LisppObject div(std::vector<type::LisppObject> args);

// I/O
type::LisppObject print(std::vector<type::LisppObject> args);

// List Processing
type::LisppObject list(std::vector<type::LisppObject> args);
type::LisppObject is_empty(std::vector<type::LisppObject> args);
type::LisppObject count(std::vector<type::LisppObject> args);
type::LisppObject first(std::vector<type::LisppObject> args);
type::LisppObject rest(std::vector<type::LisppObject> args);

// Logical
// Note: `_` prefix to avoid C++ keyword clash.
type::LisppObject _not(std::vector<type::LisppObject> args);
type::LisppObject _and(std::vector<type::LisppObject> args);
type::LisppObject _or(std::vector<type::LisppObject> args);

// Relational
type::LisppObject less(std::vector<type::LisppObject> args);
type::LisppObject less_eq(std::vector<type::LisppObject> args);
type::LisppObject greater(std::vector<type::LisppObject> args);
type::LisppObject greater_eq(std::vector<type::LisppObject> args);
type::LisppObject equal(std::vector<type::LisppObject> args);
type::LisppObject not_equal(std::vector<type::LisppObject> args);

// Type Predicates
type::LisppObject is_list(std::vector<type::LisppObject> args);
type::LisppObject is_nil(std::vector<type::LisppObject> args);
type::LisppObject is_true(std::vector<type::LisppObject> args);
type::LisppObject is_false(std::vector<type::LisppObject> args);
type::LisppObject is_symbol(std::vector<type::LisppObject> args);
type::LisppObject is_number(std::vector<type::LisppObject> args);

using CoreOperator =
    std::function<type::LisppObject(std::vector<type::LisppObject>)>;

// Core Operator Table
static std::unordered_map<std::string, CoreOperator> core = {
    // Arithmetic
    {"+", &add},
    {"-", &sub},
    {"*", &mul},
    {"/", &div},
    // I/0
    {"print", &print},
    // List Processing
    {"list", &list},
    {"empty?", &is_empty},
    {"count", &count},
    {"first", &first},
    {"rest", &rest},
    // Logical
    {"not", &_not},
    {"and", &_and},
    {"or", &_or},
    // Relational
    {"<", &less},
    {"<=", &less_eq},
    {">", &greater},
    {">=", &greater_eq},
    {"=", &equal},
    {"!=", &not_equal},
    // Type Predicates
    {"list?", &is_list},
    {"nil?", &is_nil},
    {"true?", &is_true},
    {"false?", &is_false},
    {"symbol?", &is_symbol},
    {"number?", &is_number}};

} // namespace operators

#endif // OPERATORS_H
