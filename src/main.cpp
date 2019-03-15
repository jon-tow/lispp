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
        std::string line;

        printer::welcome();
        while (true) {
                printer::prompt();
                std::getline(std::cin, line);
		try {
			auto output = interpreter::REP(line, global_env);
			printer::format_print(output);
		}
		catch (std::runtime_error err) {
			std::cout << err.what() << std::endl;
		}
        }
        return 0;
}
