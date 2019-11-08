#ifndef TYPES_H
#define TYPES_H

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

namespace type {

enum class Type { Nil, True, False, Number, String, List, Symbol, Function };

static std::unordered_map<Type, std::string> types = {
    {Type::Nil, "nil"},
    {Type::True, "true"},
    {Type::False, "false"},
    {Type::List, "list"},
};

struct LisppObject {
        Type type = Type::Nil;
        int number = 0;
        std::string symbol = "";
        std::string string = "";
        std::vector<LisppObject> items;
        std::function<LisppObject(std::vector<LisppObject>)> lambda;

        bool is_number() const { return type == Type::Number; }
        bool is_string() const { return type == Type::String; }
        bool is_symbol() const { return type == Type::Symbol; }
        bool is_list() const { return type == Type::List; }
        bool is_true() const { return type == Type::True; }
        bool is_false() const { return type == Type::False; }
        bool is_function() const { return type == Type::Function; }

        static LisppObject create_nil()
        {
                LisppObject exp = {.type = Type::Nil};
                return exp;
        }

        static LisppObject create_number(int number)
        {
                LisppObject exp = {.type = Type::Number, .number = number};
                return exp;
        }

        static LisppObject create_string(const std::string& string)
        {
                LisppObject exp = {.type = Type::String, .string = string};
                return exp;
        }

        static LisppObject create_symbol(const std::string& symbol)
        {
                LisppObject exp = {.type = Type::Symbol, .symbol = symbol};
                return exp;
        }

        static LisppObject create_list(const std::vector<LisppObject>& list)
        {
                LisppObject exp = {.type = Type::List, .items = list};
                return exp;
        }

        static LisppObject create_function(
            std::function<LisppObject(std::vector<LisppObject>)> function)
        {
                LisppObject exp = {.type = Type::Function, .lambda = function};
                return exp;
        }

        static LisppObject create_true()
        {
                LisppObject t = {.type = Type::True};
                return t;
        }

        static LisppObject create_false()
        {
                LisppObject f = {.type = Type::False};
                return f;
        }
};
} // namespace type

#endif // TYPES_H
