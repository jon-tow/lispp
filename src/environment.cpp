#include "environment.h"

using namespace type;

void Environment::set(const std::string& sym, const LisppObject& value)
{
        symbols[sym] = value;
}

LisppObject Environment::lookup(const std::string& sym)
{
        std::optional<Environment> opt = find(sym);
        if (opt.has_value()) {
                auto env = opt.value();
                return env.symbols[sym];
        }
        else {
                throw unbound_symbol_error(sym);
        }
}

std::optional<Environment> Environment::find(const std::string& sym)
{
        bool found = symbols.find(sym) != symbols.end();
        if (found) {
                return *this;
        }
        else if (parent != nullptr) {
                return parent->find(sym);
        }
        else {
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
