#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "core.h"
#include "exception.h"
#include "types.h"

class Environment {
      public:
        Environment() = default;
        Environment(std::shared_ptr<Environment> parent)
            : parent(std::move(parent))
        {
        }

        type::LisppObject lookup(const std::string &sym);
        void set(const std::string &sym, const type::LisppObject &value);
        void print_symbols();

        static Environment setup();

      private:
        std::shared_ptr<Environment> parent;
        std::unordered_map<std::string, type::LisppObject> symbols;
        std::optional<Environment> find(const std::string &sym);
};

#endif // ENVIRONMENT_H
