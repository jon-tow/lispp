#ifndef SPECIALS_H
#define SPECIALS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "types.h"

namespace syntax {

static std::unordered_map<type::Type, std::string> types = {
    { type::Type::Nil, "nil" },
    { type::Type::True, "true" },
    { type::Type::False, "false" },
    { type::Type::List, "list" }
};

enum class SpecialForm {
    definition,
    assignment,
    local_assignment,
    function,
    conditional_if,
};

static std::unordered_map<SpecialForm, std::string> specials = {
    { SpecialForm::definition, "def" },
    { SpecialForm::assignment, "set" },
    { SpecialForm::local_assignment, "let" },
    { SpecialForm::function, "fn" },
    { SpecialForm::conditional_if, "if" },
};

/* Definition Selectors */

inline bool
is_definition(const std::string& symbol)
{
    return symbol == specials[SpecialForm::definition];
}

inline type::LisppObject definition_variable(const type::LisppObject& expr)
{
    return expr.items.at(1);
}

inline type::LisppObject definition_value(const type::LisppObject& expr)
{
    return expr.items.at(2);
}

/* Assignment Selectors */

inline bool is_assigment(const std::string& symbol)
{
    return symbol == specials[SpecialForm::assignment];
}

inline type::LisppObject variable(const type::LisppObject& expr)
{
    return expr.items.at(1);
}

inline type::LisppObject variable_update(const type::LisppObject& expr)
{
    return expr.items.at(2);
}

/* Local Assignment Selectors */

inline bool is_local_assignment(const std::string& symbol)
{
    return symbol == specials[SpecialForm::local_assignment];
}

inline std::vector<type::LisppObject> local_variables(const type::LisppObject& expr)
{
    auto bindings = expr.items.at(1);
    return bindings.items;
}

inline type::LisppObject local_body(const type::LisppObject& expr)
{
    return expr.items.at(2);
}

/* Function Selectors */

inline bool is_function(const std::string& symbol)
{
    return symbol == specials[SpecialForm::function];
}

inline std::vector<type::LisppObject> function_parameters(const type::LisppObject& expr)
{
    auto params = expr.items.at(1);
    return params.items;
}

inline type::LisppObject function_body(const type::LisppObject& expr)
{
    return expr.items.at(2);
}

inline type::LisppObject get_operator(const type::LisppObject& expr)
{
    return expr.items.front();
}

inline std::vector<type::LisppObject> get_operands(const type::LisppObject& expr)
{
    std::vector<type::LisppObject> l = expr.items;
    std::vector<type::LisppObject> operands(l.begin() + 1, l.end());
    return operands;
}

/* If Selectors */

inline bool is_if(const std::string& symbol)
{
    return symbol == specials[SpecialForm::conditional_if];
}

inline type::LisppObject if_predicate(const type::LisppObject& expr)
{
    return expr.items.at(1);
}

inline type::LisppObject if_consequent(const type::LisppObject& expr)
{
    return expr.items.at(2);
}

inline type::LisppObject if_alternative(const type::LisppObject& expr)
{
    if (expr.items.size() < 4) {
        return type::LisppObject::create_nil();
    }
    return expr.items.at(3);
}

};

#endif
