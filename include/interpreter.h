#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "evaluator.h"
#include "exception.h"
#include "frame.h"
#include "printer.h"
#include "reader.h"
#include <iostream>

namespace interpreter {

std::string getinput();
std::string rep(const std::string& line, Frame& frame);
void repl();

} // namespace interpreter

#endif // INTERPRETER_H
