#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "environment.h"
#include "evaluator.h"
#include "printer.h"
#include "reader.h"

namespace interpreter {

std::string REP(const std::string& line, Environment& env);

} // namespace interpreter

#endif // INTERPRETER_H
