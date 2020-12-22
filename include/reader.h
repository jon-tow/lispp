#ifndef READER_H
#define READER_H

#include <algorithm>
#include <charconv>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "syntax.h"
#include "type.h"

class Reader {
      public:
        Reader(const std::vector<std::string>& tokens)
            : tokens{tokens}, position{0}
        {
        }

        static type::LisppObject read(const std::string& program);
        static std::vector<std::string> tokenize(const std::string& text);

        type::LisppObject read_form();
        type::LisppObject read_list();
        type::LisppObject read_string();
        type::LisppObject read_atom();

      private:
        std::vector<std::string> tokens;
        unsigned int position;

        std::optional<std::string> next();
        std::optional<std::string> peek();
        bool out_of_bounds();
};

#endif // READER_H
