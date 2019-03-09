#include "environment.h"

using namespace type;

void Environment::set(const std::string& key, const LisppObject& value)
{
    symbols[key] = value;
}

std::optional<LisppObject> Environment::lookup(const std::string& key)
{
    std::optional<Environment> opt = find(key);
    if (opt.has_value()) {
        auto env = opt.value();
        return env.symbols[key];
    }
    return std::nullopt;
}

std::optional<Environment> Environment::find(const std::string& key)
{
    bool found = symbols.find(key) != symbols.end();
    if (found) {
        return *this;
    } else if (parent != nullptr) {
        return parent->find(key);
    } else {
        return std::nullopt;
    }
}

void Environment::print_symbols()
{
    for (const auto& sym : symbols) {
        std::cout << sym.first << std::endl;
    }
}

Environment Environment::setup()
{
    Environment env;
    auto ns = core::build_core();
    for (const auto& name_op : ns) {
        auto name = name_op.first;
        auto op = LisppObject::create_function(name_op.second);
        env.set(name, op);
    }
    return env;
}
