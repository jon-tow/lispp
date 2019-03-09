#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "core.h"
#include "types.h"

class Environment {
public:
    Environment() = default;
    Environment(std::shared_ptr<Environment> parent)
        : parent(std::move(parent))
    {
    }

    std::optional<type::LisppObject> lookup(const std::string& key);
    void set(const std::string& key, const type::LisppObject& value);
    void print_symbols();

    static Environment setup();

private:
    std::shared_ptr<Environment> parent;
    std::unordered_map<std::string, type::LisppObject> symbols;

    std::optional<Environment> find(const std::string& key);
};

#endif // ENVIRONMENT_H