#include "interpreter.h"

std::string interpreter::REP(const std::string& line, Environment& env)
{
        auto expression = Reader::read(line);
        auto value = evaluator::eval(expression, env);
        auto output = printer::print(value);
        return output;
}
