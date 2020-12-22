#include <iostream>
#include <string>
#include <vector>

#include "interpreter.h"

/* Lispp Motherboard */
int main(int argc, char* argv[])
{
        if (argc == 1) {
                interpreter::repl();
        }
        else {
                // Execute file or script.
        }
}
