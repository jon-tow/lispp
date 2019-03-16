#ifndef SPECIALS_H
#define SPECIALS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "exception.h"
#include "types.h"

namespace syntax {

static std::unordered_map<type::Type, std::string> types = {
    {type::Type::Nil, "nil"},
    {type::Type::True, "true"},
    {type::Type::False, "false"},
    {type::Type::List, "list"},
};

enum class SpecialForm {
        definition,
        assignment,
        local_assignment,
        function,
        conditional_if,
};

static std::unordered_map<SpecialForm, std::string> specials = {
    {SpecialForm::definition, "def"},       {SpecialForm::assignment, "set"},
    {SpecialForm::local_assignment, "let"}, {SpecialForm::function, "fn"},
    {SpecialForm::conditional_if, "if"},
};

inline bool out_of_bounds(size_t index, size_t size) { return index >= size; }

/* Definition Selectors */

inline bool is_definition(const std::string& symbol)
{
        return symbol == specials[SpecialForm::definition];
}

inline type::LisppObject definition_variable(const type::LisppObject& expr)
{
        size_t def_pos = 1;
        if (out_of_bounds(def_pos, expr.items.size())) {
                throw ill_form_error("definition");
        }
        return expr.items.at(def_pos);
}

inline type::LisppObject definition_value(const type::LisppObject& expr)
{
        size_t def_value = 2;
        if (out_of_bounds(def_value, expr.items.size())) {
                throw ill_form_error("definition");
        }
        return expr.items.at(def_value);
}

/* Assignment Selectors */

inline bool is_assigment(const std::string& symbol)
{
        return symbol == specials[SpecialForm::assignment];
}

inline type::LisppObject variable(const type::LisppObject& expr)
{
        size_t var_pos = 1;
        if (out_of_bounds(var_pos, expr.items.size())) {
                throw ill_form_error("assignment");
        }
        return expr.items.at(var_pos);
}

inline type::LisppObject variable_update(const type::LisppObject& expr)
{
        size_t update_pos = 2;
        if (out_of_bounds(update_pos, expr.items.size())) {
                throw ill_form_error("assignment");
        }
        return expr.items.at(update_pos);
}

/* Local Assignment Selectors */

inline bool is_local_assignment(const std::string& symbol)
{
        return symbol == specials[SpecialForm::local_assignment];
}

inline std::vector<type::LisppObject>
local_variables(const type::LisppObject& expr)
{
        size_t bind_pos = 1;
        if (out_of_bounds(bind_pos, expr.items.size())) {
                throw ill_form_error("local assignment");
        }
        auto bindings = expr.items.at(bind_pos);
        return bindings.items;
}

inline type::LisppObject local_body(const type::LisppObject& expr)
{
        size_t body_pos = 2;
        if (out_of_bounds(body_pos, expr.items.size())) {
                throw ill_form_error("local assignment");
        }
        return expr.items.at(body_pos);
}

/* Function Selectors */

inline bool is_function(const std::string& symbol)
{
        return symbol == specials[SpecialForm::function];
}

inline std::vector<type::LisppObject>
function_parameters(const type::LisppObject& expr)
{
        size_t params_pos = 1;
        if (out_of_bounds(params_pos, expr.items.size())) {
                throw ill_form_error("local assignment");
        }
        auto params = expr.items.at(params_pos);
        return params.items;
}

inline type::LisppObject function_body(const type::LisppObject& expr)
{
        size_t body_pos = 2;
        if (out_of_bounds(body_pos, expr.items.size())) {
                throw ill_form_error("function");
        }
        return expr.items.at(body_pos);
}

inline type::LisppObject get_operator(const type::LisppObject& expr)
{
        if (expr.items.empty()) {
                throw ill_form_error("function");
        }
        return expr.items.front();
}

inline std::vector<type::LisppObject>
get_operands(const type::LisppObject& expr)
{
        if (expr.items.size() < 2) {
                throw ill_form_error("function");
        }
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
        size_t pred_pos = 1;
        if (out_of_bounds(pred_pos, expr.items.size())) {
                throw ill_form_error("local assignment");
        }
        return expr.items.at(pred_pos);
}

inline type::LisppObject if_consequent(const type::LisppObject& expr)
{
        size_t cons_pos = 2;
        if (out_of_bounds(cons_pos, expr.items.size())) {
                throw ill_form_error("local assignment");
        }
        return expr.items.at(cons_pos);
}

inline type::LisppObject if_alternative(const type::LisppObject& expr)
{
        size_t alt_pos = 3;
        if (expr.items.size() < 4) {
                return type::LisppObject::create_nil();
        }
        return expr.items.at(alt_pos);
}

}; // namespace syntax

#endif
