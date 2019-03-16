#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <string>

#include "types.h"

namespace printer {

std::string print(const type::LisppObject& ast);
void format_print(const std::string& ast);
void prompt();
void welcome();

} // namespace printer

#endif // PRINTER_H
