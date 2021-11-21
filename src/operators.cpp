#include "operators.h"

using namespace type;

namespace {

LisppObject equal_helper(const LisppObject& l1, const LisppObject& l2)
{
        if (l1.type != l2.type) {
                return LisppObject::create_false();
        }
        switch (l1.type) {
        case Type::Number:
                return (l1.number == l2.number) ? LisppObject::create_true()
                                                : LisppObject::create_false();
        case Type::String:
                return (l1.string == l2.string) ? LisppObject::create_true()
                                                : LisppObject::create_false();
        case Type::List:
                for (unsigned long i = 0; i < l1.items.size(); i++) {
                        auto predicate = equal_helper(l1.items[i], l2.items[i]);
                        if (predicate.is_false()) {
                                return LisppObject::create_false();
                        }
                }
                return LisppObject::create_true();
        default:
                return LisppObject::create_false();
        }
}

} // namespace

// Arithmetic

/// (+ <atom-1> ... <atom-n>) -> LisppObject.Number
LisppObject operators::add(std::vector<LisppObject> args)
{
        auto sum = 0.0;
        for (const auto& arg : args) {
                sum += arg.number;
        }
        return LisppObject::create_number(sum);
}

/// (- <atom-1> ... <atom-n>) -> LisppObject.Number
LisppObject operators::sub(std::vector<LisppObject> args)
{
        if (args.empty()) {
                throw std::invalid_argument("\n;NaN. 0 arguments given.\n");
        }
        if (args.size() == 1) {
                auto num = args.at(0).number;
                return LisppObject::create_number(num * -1);
        }
        auto diff = args.at(0).number;
        for (auto it = args.begin() + 1; it != args.end(); it++) {
                diff -= it->number;
        }
        return LisppObject::create_number(diff);
}

/// (* <atom-1> ... <atom-n>) -> LisppObject.Number
LisppObject operators::mul(std::vector<LisppObject> args)
{
        auto prod = 1.0;
        for (const auto& arg : args) {
                prod *= arg.number;
        }
        return LisppObject::create_number(prod);
}

/// (/ <atom-1> ... <atom-n>) -> LisppObject.Number
LisppObject operators::div(std::vector<LisppObject> args)
{
        if (args.empty()) {
                throw std::runtime_error("\n;NaN. 0 arguments given.\n");
        }
        if (args.size() == 1) {
                auto num = args.at(0).number;
                return LisppObject::create_number(1.0 / num);
        }
        auto quotient = args.at(0).number;
        for (auto it = args.begin() + 1; it != args.end(); it++) {
                if (it->number == 0) {
                        throw std::runtime_error(
                            "\n;Infinity. Division by zero.\n");
                }
                quotient /= it->number;
        }
        return LisppObject::create_number(quotient);
}

// I/O

/// (print <any>) -> LisppObject.Nil
LisppObject operators::print(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(print <any>)", 1, args.size());
        }
        std::cout << printer::print(args.at(0));
        return LisppObject::create_nil();
}

// List Processing

// (list <atom-1 | list-1> ... <atom-n | list-n> ) -> LisppObject.List
LisppObject operators::list(std::vector<LisppObject> args)
{
        return LisppObject::create_list(args);
}

/// (empty? <list>) -> LisppObject.True | LisppObject.False
LisppObject operators::is_empty(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(empty? <list>)", 1, args.size());
        }
        auto list = args.front();
        return list.items.empty() ? LisppObject::create_true()
                                  : LisppObject::create_false();
}

/// (count <list>) -> LisppObject.Number
LisppObject operators::count(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(count <list>)", 1, args.size());
        }
        auto list = args.front();
        auto count = list.items.size();
        return LisppObject::create_number(count); // (size_t -> double) cast
}

/// (first <list>) -> LisppObject
LisppObject operators::first(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(first <list>)", 1, args.size());
        }

        auto list = args.front();
        if (list.items.empty()) {
                throw std::runtime_error(
                    "\n;Empty List. Cannot return the (first <list>) "
                    "element.\n");
        }
        return list.items.front();
}

/// (rest <list>) -> (list LisppObject)
LisppObject operators::rest(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(rest <list>)", 1, args.size());
        }

        auto list = args.front();
        if (list.items.empty()) {
                throw std::runtime_error(
                    "\n;Empty List. Cannot return the (rest <list>) "
                    "of the elements.\n");
        }
        if (list.items.size() == 1) {
                return LisppObject::create_list({});
        }
        std::vector<LisppObject> rest{list.items.begin() + 1, list.items.end()};
        return LisppObject::create_list(rest);
}

// Logical

/// (not <any>) -> LisppObject.True | LisppObject.False
LisppObject operators::_not(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(not <list>)", args.size(), 1);
        }
        return (args.front().type == Type::False) ? LisppObject::create_true()
                                                  : LisppObject::create_false();
}

/// (and <bool-1> ... <bool-n>) -> LisppObject.True | LisppObject.False
LisppObject operators::_and(std::vector<LisppObject> args)
{
        for (const auto& arg : args) {
                if (arg.is_false()) {
                        return LisppObject::create_false();
                }
        }
        return LisppObject::create_true();
}

/// (and <bool-1> ... <bool-n>) -> LisppObject.True | LisppObject.False
LisppObject operators::_or(std::vector<LisppObject> args)
{
        for (const auto& arg : args) {
                if (arg.is_true()) {
                        return LisppObject::create_true();
                }
        }
        return LisppObject::create_false();
}

// Relational

/// (< <atom-1> ... <atom-n>) -> LisppObject.True | LisppObject.False
LisppObject operators::less(std::vector<LisppObject> args)
{
        for (auto it = args.begin(); it != args.end() - 1; it++) {
                if (it->number >= (it + 1)->number) {
                        return LisppObject::create_false();
                }
        }
        return LisppObject::create_true();
}

/// (<= <atom-1> ... <atom-n>) -> LisppObject.True | LisppObject.False
LisppObject operators::less_eq(std::vector<LisppObject> args)
{
        for (auto it = args.begin(); it != args.end() - 1; it++) {
                if (it->number > (it + 1)->number) {
                        return LisppObject::create_false();
                }
        }
        return LisppObject::create_true();
}

/// (> <atom-1> ... <atom-n>) -> LisppObject.True | LisppObject.False
LisppObject operators::greater(std::vector<LisppObject> args)
{
        for (auto it = args.begin(); it != args.end() - 1; it++) {
                if (it->number <= (it + 1)->number) {
                        return LisppObject::create_false();
                }
        }
        return LisppObject::create_true();
}

/// (>= <atom-1> ... <atom-n>) -> LisppObject.True | LisppObject.False
LisppObject operators::greater_eq(std::vector<LisppObject> args)
{
        for (auto it = args.begin(); it != args.end() - 1; it++) {
                if (it->number < (it + 1)->number) {
                        return LisppObject::create_false();
                }
        }
        return LisppObject::create_true();
}

/// (= <atom-1> ... <atom-n>) -> LisppObject.True | LisppObject.False
LisppObject operators::equal(std::vector<LisppObject> args)
{
        for (auto it = args.begin(); it != args.end() - 1; it++) {
                LisppObject eq = equal_helper(*it, *(it + 1));
                if (eq.is_false()) {
                        return LisppObject::create_false();
                }
        }
        return LisppObject::create_true();
}

/// (!= <atom-1> ... <atom-n>) -> LisppObject.True | LisppObject.False
LisppObject operators::not_equal(std::vector<LisppObject> args)
{
        return _not({equal(args)});
}

// Type Predicates

/// (list? <list>) -> LisppObject.True | LisppObject.False
LisppObject operators::is_list(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(list? <list>)", 1, args.size());
        }
        auto list = args.front();
        return list.is_list() ? LisppObject::create_true()
                              : LisppObject::create_false();
}

/// (nil? <any>) -> LisppObject.True | LisppObject.False
LisppObject operators::is_nil(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(nil? <any>)", 1, args.size());
        }
        auto any = args.front();
        return any.is_nil() ? LisppObject::create_true()
                            : LisppObject::create_false();
}

/// (true? <any>) -> LisppObject.True | LisppObject.False
LisppObject operators::is_true(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(true? <any>)", 1, args.size());
        }
        auto any = args.front();
        return any.is_true() ? LisppObject::create_true()
                             : LisppObject::create_false();
}

/// (false? <any>) -> LisppObject.True | LisppObject.False
LisppObject operators::is_false(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(false? <any>)", 1, args.size());
        }
        auto any = args.front();
        return any.is_false() ? LisppObject::create_true()
                              : LisppObject::create_false();
}

/// (symbol? <any>) -> LisppObject.True | LisppObject.False
LisppObject operators::is_symbol(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(symbol? <any>)", 1, args.size());
        }
        auto any = args.front();
        return any.is_symbol() ? LisppObject::create_true()
                               : LisppObject::create_false();
}

/// (number? <any>) -> LisppObject.True | LisppObject.False
LisppObject operators::is_number(std::vector<LisppObject> args)
{
        if (args.size() != 1) {
                throw exception::invalid_arg_size(
                        "(number? <any>)", 1, args.size());
        }
        auto any = args.front();
        return any.is_number() ? LisppObject::create_true()
                               : LisppObject::create_false();
}
