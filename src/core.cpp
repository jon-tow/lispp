#include "core.h"

using namespace type;

namespace {

void set_arithmetic(std::unordered_map<std::string, core::CoreFunction>& ns)
{
    ns["+"] = &core::add;
    ns["-"] = &core::sub;
    ns["*"] = &core::mul;
    ns["/"] = &core::div;
}

void set_list_processing(std::unordered_map<std::string, core::CoreFunction>& ns)
{
    ns["list"] = &core::list;
    ns["list?"] = &core::is_list;
    ns["empty?"] = &core::is_empty;
    ns["count"] = &core::count;
}

void set_logic(std::unordered_map<std::string, core::CoreFunction>& ns)
{
    ns["="] = &core::equal;
    ns["<"] = &core::less;
    ns["<="] = &core::less_eq;
    ns[">"] = &core::greater;
    ns[">="] = &core::greater_eq;
}

LisppObject equal_helper(const LisppObject& l1, const LisppObject& l2)
{
    switch (l1.type) {
    case Type::Number:
        return (l1.number == l2.number)
            ? LisppObject::create_true()
            : LisppObject::create_false();
    case Type::String:
        return (l1.str == l2.str)
            ? LisppObject::create_true()
            : LisppObject::create_false();
    case Type::List:
        if (l1.items.size() != l2.items.size()) {
            return LisppObject::create_false();
        }
        for (unsigned long i = 0; i < l1.items.size(); i++) {
            auto predicate = equal_helper(l1.items[i], l2.items[i]);
            if (predicate.type == Type::False) {
                return LisppObject::create_false();
            }
        }
        return LisppObject::create_true();
    default:
        return LisppObject::create_false();
    }
}

} // namespace

std::unordered_map<std::string, core::CoreFunction>
core::build_core()
{
    std::unordered_map<std::string, core::CoreFunction> ns;
    set_arithmetic(ns);
    set_list_processing(ns);
    set_logic(ns);
    return ns;
}

/* Arithmetic */

LisppObject core::add(std::vector<LisppObject> args)
{
    auto sum = 0;
    for (const auto& i : args) {
        sum += i.number;
    }
    return LisppObject::create_number(sum);
}

LisppObject core::sub(std::vector<LisppObject> args)
{
    auto diff = args[0].number;
    for (auto i = args.begin() + 1; i != args.end(); i++) {
        diff -= i->number;
    }
    return LisppObject::create_number(diff);
}

LisppObject core::mul(std::vector<LisppObject> args)
{
    auto prod = 1;
    for (const auto& i : args) {
        prod *= i.number;
    }
    return LisppObject::create_number(prod);
}

LisppObject core::div(std::vector<LisppObject> args)
{
    auto quotient = args[0].number;
    for (auto i = args.begin() + 1; i != args.end(); i++) {
        quotient /= i->number;
    }
    return LisppObject::create_number(quotient);
}

/* List Processing */

LisppObject core::list(std::vector<LisppObject> args)
{
    return LisppObject::create_list(args);
}

LisppObject core::is_list(std::vector<LisppObject> args)
{
    auto list = args.front();
    if (list.is_list()) {
        return LisppObject::create_true();
    }
    return LisppObject::create_false();
}

LisppObject core::is_empty(std::vector<LisppObject> args)
{
    auto list = args.front();
    if (list.items.empty()) {
        return LisppObject::create_true();
    }
    return LisppObject::create_false();
}

LisppObject core::count(std::vector<LisppObject> args)
{
    auto list = args.front();
    auto count = list.items.size();
    return LisppObject::create_number(count); // (size_t -> int) cast
}

/* Logic */

LisppObject core::less(std::vector<LisppObject> args)
{
    for (auto i = args.begin(); i != args.end() - 1; i++) {
        if (i->number >= (i + 1)->number) {
            return LisppObject::create_false();
        }
    }
    return LisppObject::create_true();
}

LisppObject core::less_eq(std::vector<LisppObject> args)
{
    for (auto i = args.begin(); i != args.end() - 1; i++) {
        if (i->number > (i + 1)->number) {
            return LisppObject::create_false();
        }
    }
    return LisppObject::create_true();
}

LisppObject core::greater(std::vector<LisppObject> args)
{
    for (auto i = args.begin(); i != args.end() - 1; i++) {
        if (i->number <= (i + 1)->number) {
            return LisppObject::create_false();
        }
    }
    return LisppObject::create_true();
}

LisppObject core::greater_eq(std::vector<LisppObject> args)
{
    for (auto i = args.begin(); i != args.end() - 1; i++) {
        if (i->number < (i + 1)->number) {
            return LisppObject::create_false();
        }
    }
    return LisppObject::create_true();
}

LisppObject core::equal(std::vector<LisppObject> args)
{
    if (args[0].type != args[1].type) {
        return LisppObject::create_false();
    }
    auto arg1 = args[0];
    auto arg2 = args[1];
    return equal_helper(arg1, arg2);
}
