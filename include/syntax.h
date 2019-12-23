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
    "(list|def|if|let|set|fn)|([-+*/^%~=])|\"(.)*\"|(\\d)+|\\w+|\\(|\\)";

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

inline bool out_of_bounds(size_t index, size_t size) { return index >= size; }

// Definition Selectors

inline bool is_definition(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::definition];
}

inline type::LisppObject definition_name(const type::LisppObject& expr)
{
        // def_pos: 1
        // (def <name> <value>)
        // _^___^______^______
        //  0   1      2
        size_t name_pos = 1;
        if (out_of_bounds(name_pos, expr.items.size())) {
                throw ill_form_error("missing definition name\n\n"
                                     "(def <name> <value>)\n"
                                     "_____^______________");
        }
        return expr.items.at(name_pos);
}

inline type::LisppObject definition_value(const type::LisppObject& expr)
{
        // def_pos: 2
        // (def <name> <value>)
        // _^___^______^_______
        //  0   1      2
        size_t value_pos = 2;
        if (out_of_bounds(value_pos, expr.items.size())) {
                throw ill_form_error("missing definition value\n\n"
                                     "(def <name> <value>)\n"
                                     "____________^_______");
        }
        return expr.items.at(value_pos);
}

// Assignment Selectors

inline bool is_assigment(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::assignment];
}

inline type::LisppObject variable_name(const type::LisppObject& expr)
{
        // name_pos: 1
        // (set <name> <update>)
        // _^___^______^________
        //  0   1      2
        size_t name_pos = 1;
        if (out_of_bounds(name_pos, expr.items.size())) {
                throw ill_form_error("missing variable name\n\n"
                                     "(set <name> <update>)\n"
                                     "_____^_______________");
        }
        return expr.items.at(name_pos);
}

inline type::LisppObject variable_update(const type::LisppObject& expr)
{
        // update_pos: 2
        // (set <name> <update>)
        // _^___^______^________
        //  0   1      2
        size_t update_pos = 2;
        if (out_of_bounds(update_pos, expr.items.size())) {
                throw ill_form_error("missing variable update\n\n"
                                     "(set <name> <update>)\n"
                                     "____________^________");
        }
        return expr.items.at(update_pos);
}

// Local Assignment Selectors

inline bool is_local_assignment(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::local_assignment];
}

inline std::vector<type::LisppObject>
local_variables(const type::LisppObject& expr)
{
        // vars_pos: 1
        // (let (<name> <value>) <body>)
        // _^___^________________^______
        //  0   1                2
        size_t vars_pos = 1;
        if (out_of_bounds(vars_pos, expr.items.size())) {
                throw ill_form_error("missing local names.\n\n"
                                     "(let (<name> <value>) <body>)\n"
                                     "_____^_______________________");
        }
        auto vars = expr.items.at(vars_pos);
        return vars.items;
}

inline type::LisppObject local_body(const type::LisppObject& expr)
{
        // body_pos: 2
        // (let (<name> <value>) <body>)
        // _^___^________________^______
        //  0   1                2
        size_t body_pos = 2;
        if (out_of_bounds(body_pos, expr.items.size())) {
                throw ill_form_error("missing local assignment body.\n\n"
                                     "(let (<name> <value>) <body>)\n"
                                     "______________________^______");
        }
        return expr.items.at(body_pos);
}

// Function Selectors

inline bool is_function(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::function];
}

inline std::vector<type::LisppObject>
function_parameters(const type::LisppObject& expr)
{
        // params_pos: 1
        // (fn  (<parameters>) <body>)
        // _^___^______________^______
        //  0   1              2
        size_t params_pos = 1;
        if (out_of_bounds(params_pos, expr.items.size())) {
                throw ill_form_error("missing function paramters.\n\n"
                                     "(fn (<parameters>) <body>)\n"
                                     "____^_____________________");
        }
        auto params = expr.items.at(params_pos);
        return params.items;
}

inline type::LisppObject function_body(const type::LisppObject& expr)
{
        // body_pos: 2
        // (fn  (<parameters>) <body>)
        // _^___^______________^______
        //  0   1              2
        size_t body_pos = 2;
        if (out_of_bounds(body_pos, expr.items.size())) {
                throw ill_form_error("missing function body.\n\n"
                                     "(fn (<parameters>) <body>)\n"
                                     "___________________^______");
        }
        return expr.items.at(body_pos);
}

// Apply Selectors

inline type::LisppObject apply_operator(const type::LisppObject& expr)
{
        auto op = expr.items.front();
        if (!op.is_function()) {
                throw ill_form_error("object is not callable");
        }
        return op;
}

inline std::vector<type::LisppObject>
apply_operands(const type::LisppObject& expr)
{
        // TODO: Make sure number operands are same as what's expected?
        std::vector<type::LisppObject> l = expr.items;
        std::vector<type::LisppObject> operands(l.begin() + 1, l.end());
        return operands;
}

// If Selectors

inline bool is_if(const std::string& symbol)
{
        return symbol == keywords[KeywordKind::conditional_if];
}

inline type::LisppObject if_predicate(const type::LisppObject& expr)
{
        // pred_pos: 1
        // (if (<predicate>) <consequent> <?-alternative>)
        // _^___^____________^____________^_____________
        //  0   1            2            3
        size_t pred_pos = 1;
        if (out_of_bounds(pred_pos, expr.items.size())) {
                throw ill_form_error(
                    "missing predicate.\n\n"
                    "(if (<predicate>) <consequent> <?-alternative>)\n"
                    "_____^_________________________________________");
        }
        return expr.items.at(pred_pos);
}

inline type::LisppObject if_consequent(const type::LisppObject& expr)
{
        // conseq_pos: 2
        // (if (<predicate>) <consequent> <?-alternative>)
        // _^___^____________^____________^______________
        //  0   1            2            3
        size_t conseq_pos = 2;
        if (out_of_bounds(conseq_pos, expr.items.size())) {
                throw ill_form_error(
                    "missing consequent.\n\n"
                    "(if (<predicate>) <consequent> <?-alternative>)\n"
                    "__________________^____________________________");
        }
        return expr.items.at(conseq_pos);
}

inline type::LisppObject if_alternative(const type::LisppObject& expr)
{
        size_t alt_pos = 3;
        if (expr.items.size() < 4) {
                return type::LisppObject::create_nil();
        }
        return expr.items.at(alt_pos);
}

} // namespace syntax

#endif // SYNTAX_H
