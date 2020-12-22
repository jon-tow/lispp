#include "interpreter.h"

std::string interpreter::getinput()
{
        std::string input;
        std::getline(std::cin, input);
        return input;
}

std::string interpreter::rep(const std::string& line, Frame& frame)
{
        auto expression = Reader::read(line);
        auto value = evaluator::eval(expression, frame);
        auto output = printer::print(value);
        return output;
}

void interpreter::repl()
{
        Frame global_frame{Frame::global()};
        printer::welcome();
        std::string input;
        while (true) {
                printer::prompt();
                input = interpreter::getinput();
                try {
                        auto output = interpreter::rep(input, global_frame);
                        printer::format_print(output);
                }
                catch (std::runtime_error err) {
                        std::cout << err.what() << std::endl;
                }
        }
}
