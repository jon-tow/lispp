#include "frame.h"

using namespace type;

void Frame::set(const std::string& sym, const LisppObject& value)
{
        symbols[sym] = value;
}

LisppObject Frame::lookup(const std::string& sym) const
{
        std::optional<Frame> opt{find(sym)};
        if (opt.has_value()) {
                auto frame = opt.value();
                return frame.symbols[sym];
        }
        else {
                throw unbound_symbol_error(sym);
        }
}

std::optional<Frame> Frame::find(const std::string& sym) const
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

void Frame::print_symbols() const
{
        for (const auto& [sym, _] : symbols) {
                std::cout << sym << std::endl;
        }
}

Frame Frame::global()
{
        Frame global;
        for (const auto& [sym, op] : operators::core) {
                auto function = LisppObject::create_function(op);
                global.set(sym, function);
        }
        return global;
}
