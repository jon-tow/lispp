#include "evaluator.h"

using namespace type;

namespace {

LisppObject eval_symbol(const LisppObject& ast, Environment& env)
{
    auto symbol = env.lookup(ast.symbol);
    if (symbol.has_value()) {
        return symbol.value();
    } else {
        std::string err = "\n;Unbound symbol: " + ast.symbol + "\n";
        throw std::runtime_error(err);
    }
}

LisppObject eval_list(const LisppObject& ast, Environment& env)
{
    auto list = LisppObject::create_list({});
    for (const auto& item : ast.items) {
        auto value = evaluator::eval(item, env);
        list.items.push_back(value);
    }
    return list;
}

LisppObject eval_ast(const LisppObject& ast, Environment& env)
{
    switch (ast.type) {
    case Type::Symbol:
        return eval_symbol(ast, env);
    case Type::List:
        return eval_list(ast, env);
    default:
        return ast;
    }
}

LisppObject eval_definition(const LisppObject& ast, Environment& env)
{
    auto name = syntax::definition_variable(ast);
    auto value_arg = syntax::definition_value(ast);
    auto value = evaluator::eval(value_arg, env);
    env.set(name.symbol, value);
    return value;
}

LisppObject eval_assignment(const LisppObject& ast, Environment& env)
{
    auto var = syntax::variable(ast);
    auto current = env.lookup(var.symbol);
    if (!current.has_value()) {
        std::string err = "\n;Unbound symbol: " + var.symbol + "\n";
        throw std::runtime_error(err);
    }
    auto update = syntax::variable_update(ast);
    auto new_value = evaluator::eval(update, env);
    env.set(var.symbol, new_value);
    return new_value;
}

LisppObject eval_local_assignment(const LisppObject& ast, Environment& env)
{
    Environment local(std::make_shared<Environment>(env));
    auto vars = syntax::local_variables(ast);
    for (auto i = vars.begin(); i != vars.end(); i += 2) {
        auto name = *i;
        auto binding = *(i + 1);
        auto value = evaluator::eval(binding, local);
        local.set(name.symbol, value);
    }
    auto body = syntax::local_body(ast);
    return evaluator::eval(body, local);
}

LisppObject eval_if(const LisppObject& ast, Environment& env)
{
    auto predicate = syntax::if_predicate(ast);
    predicate = evaluator::eval(predicate, env);
    if (predicate.is_true()) {
        auto consequent = syntax::if_consequent(ast);
        return evaluator::eval(consequent, env);
    }
    auto alternative = syntax::if_alternative(ast);
    return evaluator::eval(alternative, env);
}

LisppObject eval_function(const LisppObject& ast, Environment& env)
{
    auto fn = [ast, &env](std::vector<LisppObject> args) {
        Environment local(std::make_shared<Environment>(env));
        auto params = syntax::function_parameters(ast);
        auto body = syntax::function_body(ast);
        if (args.size() != params.size()) {
            std::string err = "\n;The procedure has been called with "
                + std::to_string(args.size()) + " arguments; it requires "
                + std::to_string(params.size()) + ".\n";
            throw std::runtime_error(err);
        }
        for (size_t i = 0; i < params.size(); i++) {
            auto param = params.at(i);
            auto arg = args.at(i);
            auto value = evaluator::eval(arg, local);
            local.set(param.symbol, value);
        }
        return evaluator::eval(body, local);
    };
    return LisppObject::create_function(fn);
}

bool is_self_evaluating(const LisppObject& ast)
{
    return ast.is_number()
        || ast.is_string()
        || ast.is_symbol();
}

} // namespace

LisppObject evaluator::eval(const LisppObject& ast, Environment& env)
{
    if (is_self_evaluating(ast)) {
        return eval_ast(ast, env);
    }

    auto list = ast.items;
    if (list.empty()) {
        return ast;
    }

    auto symbol = list.front().symbol;
    if (syntax::is_definition(symbol)) {
        return eval_definition(ast, env);
    } else if (syntax::is_assigment(symbol)) {
        return eval_assignment(ast, env);
    } else if (syntax::is_local_assignment(symbol)) {
        return eval_local_assignment(ast, env);
    } else if (syntax::is_if(symbol)) {
        return eval_if(ast, env);
    } else if (syntax::is_function(symbol)) {
        return eval_function(ast, env);
    } else {
        auto list = eval_ast(ast, env);
        auto procedure = syntax::get_operator(list);
        auto args = syntax::get_operands(list);
        return evaluator::apply(procedure, args);
    }
}

LisppObject evaluator::apply(
    const LisppObject& procedure,
    const std::vector<LisppObject>& arguments)
{
    auto result = procedure.lambda(arguments);
    return result;
}
