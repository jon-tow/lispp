#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <string>

#include "types.h"

namespace printer {

void prompt();
void welcome();
void format_print(const std::string &ast);
std::string print(const type::LisppObject &ast);

} // namespace printer

#endif // PRINTER_H
