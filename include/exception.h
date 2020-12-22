#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>

class unbound_symbol_error : public std::runtime_error {
      public:
        unbound_symbol_error(const std::string& what)
            : std::runtime_error("\n;Unbound symbol: " + what + "\n")
        {
        }
};

class ill_form_error : public std::runtime_error {
      public:
        ill_form_error(const std::string& what)
            : std::runtime_error("\n;Ill formed special: " + what + ".\n")
        {
        }
};

class invalid_arg_size : public std::runtime_error {
      public:
        invalid_arg_size(const std::string& name, size_t size,
                         size_t expected_size)
            : std::runtime_error("\n; " + name + " has been called with " +
                                 std::to_string(size) +
                                 " arguments; it requires " +
                                 std::to_string(expected_size) + ".\n")
        {
        }
};

#endif // EXCEPTION_H
