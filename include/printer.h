#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <string>

#include "types.h"

namespace printer {

void prompt();
void welcome();
void print(const type::LisppObject& ast);

}

#endif // PRINTER_H