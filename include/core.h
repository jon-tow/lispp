#ifndef CORE_H
#define CORE_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "types.h"

namespace core {

using CoreFunction = std::function<
    type::LisppObject(std::vector<type::LisppObject>)>;

std::unordered_map<std::string, CoreFunction> build_core();

/* Arithmetic */
type::LisppObject add(std::vector<type::LisppObject> args);
type::LisppObject sub(std::vector<type::LisppObject> args);
type::LisppObject mul(std::vector<type::LisppObject> args);
type::LisppObject div(std::vector<type::LisppObject> args);

/* List Processing */
type::LisppObject list(std::vector<type::LisppObject> args);
type::LisppObject is_list(std::vector<type::LisppObject> args);
type::LisppObject is_empty(std::vector<type::LisppObject> args);
type::LisppObject count(std::vector<type::LisppObject> args);

/* Logic */
type::LisppObject equal(std::vector<type::LisppObject> args);
type::LisppObject less(std::vector<type::LisppObject> args);
type::LisppObject less_eq(std::vector<type::LisppObject> args);
type::LisppObject greater(std::vector<type::LisppObject> args);
type::LisppObject greater_eq(std::vector<type::LisppObject> args);

}
#endif // CORE_H