#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>

#include "evaluator.h"
#include "frame.h"
#include "printer.h"
#include "reader.h"

namespace interpreter {

std::string getinput();
std::string rep(const std::string& line, Frame& frame);
void repl();

} // namespace interpreter

#endif // INTERPRETER_H
