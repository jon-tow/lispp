#include <iostream>
#include <string>
#include <vector>

#include "environment.h"
#include "interpreter.h"
#include "printer.h"

/* Lispp REPL */
int main()
{
        Environment global_env = Environment::setup();
        std::string input;

        printer::welcome();
        while (true) {
                printer::prompt();
                input = interpreter::getinput();
                try {
                        auto output = interpreter::REP(input, global_env);
                        printer::format_print(output);
                }
                catch (std::runtime_error err) {
                        std::cout << err.what() << std::endl;
                }
        }
        return 0;
}
