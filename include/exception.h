#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>

class unbound_symbol_error : public std::runtime_error {
      public:
        unbound_symbol_error(const std::string &what)
            : std::runtime_error("\n;Unbound symbol: " + what + ".\n")
        {
        }
};

class ill_form_error : public std::runtime_error {
      public:
        ill_form_error(const std::string &what)
            : std::runtime_error("\n;Ill formed special: " + what + ".\n")
        {
        }
};

#endif
