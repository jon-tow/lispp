#ifndef keywords_H
#define keywords_H

#include <string>
#include <unordered_map>
#include <vector>

#include "exception.h"
#include "type.h"

namespace syntax {

// Excuse my poor regex-ing...
static inline const std::string grammar =
    "(-?\\d+\\.?\\d*)|(<=|>=|<|>|[-+*/^%~=])|(\"(.)*\")|(\\w+\\?*)|(\\(|\\))";

// Keyword Syntax

enum class KeywordKind {
        definition,
        assignment,
        local_assignment,
        function,
        conditional_if,
};

static std::unordered_map<KeywordKind, std::string> keywords = {
    {KeywordKind::definition, "def"},       {KeywordKind::assignment, "set"},
    {KeywordKind::local_assignment, "let"}, {KeywordKind::function, "fn"},
    {KeywordKind::conditional_if, "if"},
};

// Helper Function

inline bool out_of_bounds(size_t index, size_t size) { return index >= size; }

// Definition Selectors

inline bool is_definition(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::definition];
}

inline type::LisppObject definition_name(const type::LisppObject& expression)
{
        // name_pos: 1
        // (def <name> <value>)
        // _^___^______^______
        //  0   1      2
        size_t name_pos = 1;
        if (out_of_bounds(name_pos, expression.items.size())) {
                throw ill_form_error("missing definition name\n\n"
                                     "(def <name> <value>)\n"
                                     "_____^______________");
        }
        return expression.items.at(name_pos);
}

inline type::LisppObject definition_value(const type::LisppObject& expression)
{
        // def_pos: 2
        // (def <name> <value>)
        // _^___^______^_______
        //  0   1      2
        size_t value_pos = 2;
        if (out_of_bounds(value_pos, expression.items.size())) {
                throw ill_form_error("missing definition value\n\n"
                                     "(def <name> <value>)\n"
                                     "____________^_______");
        }
        return expression.items.at(value_pos);
}

// Assignment Selectors

inline bool is_assigment(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::assignment];
}

inline type::LisppObject variable_name(const type::LisppObject& expression)
{
        // name_pos: 1
        // (set <name> <update>)
        // _^___^______^________
        //  0   1      2
        size_t name_pos = 1;
        if (out_of_bounds(name_pos, expression.items.size())) {
                throw ill_form_error("missing variable name\n\n"
                                     "(set <name> <update>)\n"
                                     "_____^_______________");
        }
        return expression.items.at(name_pos);
}

inline type::LisppObject variable_update(const type::LisppObject& expression)
{
        // update_pos: 2
        // (set <name> <update>)
        // _^___^______^________
        //  0   1      2
        size_t update_pos = 2;
        if (out_of_bounds(update_pos, expression.items.size())) {
                throw ill_form_error("missing variable update\n\n"
                                     "(set <name> <update>)\n"
                                     "____________^________");
        }
        return expression.items.at(update_pos);
}

// Local Assignment Selectors

inline bool is_local_assignment(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::local_assignment];
}

inline std::vector<type::LisppObject>
local_variables(const type::LisppObject& expression)
{
        // variables_pos: 1
        // (let (<name> <value>) <body>)
        // _^___^________________^______
        //  0   1                2
        size_t variables_pos = 1;
        if (out_of_bounds(variables_pos, expression.items.size())) {
                throw ill_form_error("missing local names.\n\n"
                                     "(let (<name> <value>) <body>)\n"
                                     "_____^_______________________");
        }
        auto variables = expression.items.at(variables_pos);
        return variables.items;
}

inline type::LisppObject local_body(const type::LisppObject& expression)
{
        // body_pos: 2
        // (let (<name> <value>) <body>)
        // _^___^________________^______
        //  0   1                2
        size_t body_pos = 2;
        if (out_of_bounds(body_pos, expression.items.size())) {
                throw ill_form_error("missing local assignment body.\n\n"
                                     "(let (<name> <value>) <body>)\n"
                                     "______________________^______");
        }
        return expression.items.at(body_pos);
}

// Function Selectors

inline bool is_function(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::function];
}

inline std::vector<type::LisppObject>
function_parameters(const type::LisppObject& expression)
{
        // parameters_pos: 1
        // (fn  (<parameters>) <body>)
        // _^___^______________^______
        //  0   1              2
        size_t parameters_pos = 1;
        if (out_of_bounds(parameters_pos, expression.items.size())) {
                throw ill_form_error("missing function paramters.\n\n"
                                     "(fn (<parameters>) <body>)\n"
                                     "____^_____________________");
        }
        auto parameters = expression.items.at(parameters_pos);
        return parameters.items;
}

inline type::LisppObject function_body(const type::LisppObject& expression)
{
        // body_pos: 2
        // (fn  (<parameters>) <body>)
        // _^___^______________^______
        //  0   1              2
        size_t body_pos = 2;
        if (out_of_bounds(body_pos, expression.items.size())) {
                throw ill_form_error("missing function body.\n\n"
                                     "(fn (<parameters>) <body>)\n"
                                     "___________________^______");
        }
        return expression.items.at(body_pos);
}

// Apply Selectors

inline type::LisppObject apply_function(const type::LisppObject& expression)
{
        // function_pos: 0 (front)
        // (<function-name> <arg-1> ... <arg-n>)
        // _^_______________^___________^_______
        //  0               1           n
        auto function = expression.items.front();
        if (!function.is_function()) {
                throw ill_form_error("object is not callable");
        }
        return function;
}

inline std::vector<type::LisppObject>
apply_arguments(const type::LisppObject& expression)
{
        // arguments_pos: [1, n]
        // (<function-name> <arg-1> ... <arg-n>)
        // _^_______________^___________^_______
        //  0               1           n
        // TODO: Make sure number operands are same as what's expected?
        std::vector<type::LisppObject> l = expression.items;
        std::vector<type::LisppObject> arguments(l.begin() + 1, l.end());
        return arguments;
}

// If Selectors

inline bool is_if(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::conditional_if];
}

inline type::LisppObject if_predicate(const type::LisppObject& expression)
{
        // predicate_pos: 1
        // (if (<predicate>) <consequent> <?-alternative>)
        // _^___^____________^____________^_____________
        //  0   1            2            3
        size_t predicate_pos = 1;
        if (out_of_bounds(predicate_pos, expression.items.size())) {
                throw ill_form_error(
                    "missing predicate.\n\n"
                    "(if (<predicate>) <consequent> <?-alternative>)\n"
                    "_____^_________________________________________");
        }
        return expression.items.at(predicate_pos);
}

inline type::LisppObject if_consequent(const type::LisppObject& expression)
{
        // consequent_pos: 2
        // (if (<predicate>) <consequent> <?-alternative>)
        // _^___^____________^____________^______________
        //  0   1            2            3
        size_t consequent_pos = 2;
        if (out_of_bounds(consequent_pos, expression.items.size())) {
                throw ill_form_error(
                    "missing consequent.\n\n"
                    "(if (<predicate>) <consequent> <?-alternative>)\n"
                    "__________________^____________________________");
        }
        return expression.items.at(consequent_pos);
}

inline type::LisppObject if_alternative(const type::LisppObject& expression)
{
        size_t alternative_pos = 3;
        if (expression.items.size() < 4) {
                return type::LisppObject::create_nil();
        }
        return expression.items.at(alternative_pos);
}

} // namespace syntax

#endif // SYNTAX_H
