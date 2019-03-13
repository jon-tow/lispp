#include <iostream>
#include <string>
#include <vector>

#include "evaluator.h"
#include "printer.h"
#include "reader.h"
#include "types.h"

/* Lispp REPL */
int main()
{
        Environment global_env = Environment::setup();
        std::string line;

        printer::welcome();
        while (true) {
                printer::prompt();
                std::getline(std::cin, line);
                try {
                        auto expression = Reader::read(line);
                        auto value = evaluator::eval(expression, global_env);
                        printer::print(value);
                }
                catch (std::runtime_error err) {
                        std::cout << err.what() << std::endl;
                }
        }
        return 0;
}
