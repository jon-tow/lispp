#include "evaluator.h"

using namespace type;

namespace {

LisppObject eval_symbol(const LisppObject& ast, Environment& env)
{
        return env.lookup(ast.symbol);
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
        LisppObject name = syntax::definition_name(ast);
        LisppObject value_arg = syntax::definition_value(ast);
        LisppObject value = evaluator::eval(value_arg, env);
        env.set(name.symbol, value);
        return value;
}

LisppObject eval_assignment(const LisppObject& ast, Environment& env)
{
        LisppObject name = syntax::variable_name(ast);
        LisppObject update_arg = syntax::variable_update(ast);
        LisppObject update = evaluator::eval(update_arg, env);
        env.set(name.symbol, update);
        return update;
}

LisppObject eval_local_assignment(const LisppObject& ast, Environment& env)
{
        Environment local(std::make_shared<Environment>(env));
        std::vector<LisppObject> vars = syntax::local_variables(ast);
        for (auto it = vars.begin(); it != vars.end(); it += 2) {
                auto name = *it;
                auto binding = *(it + 1);
                auto value = evaluator::eval(binding, local);
                local.set(name.symbol, value);
        }
        LisppObject body = syntax::local_body(ast);
        return evaluator::eval(body, local);
}

LisppObject eval_if(const LisppObject& ast, Environment& env)
{
        LisppObject predicate = syntax::if_predicate(ast);
        LisppObject predicate_value = evaluator::eval(predicate, env);
        if (predicate_value.is_true()) {
                LisppObject consequent = syntax::if_consequent(ast);
                return evaluator::eval(consequent, env);
        }
        else {
                LisppObject alternative = syntax::if_alternative(ast);
                return evaluator::eval(alternative, env);
        }
}

LisppObject eval_function(const LisppObject& ast, Environment& env)
{
        std::vector<LisppObject> parameters = syntax::function_parameters(ast);
        LisppObject body = syntax::function_body(ast);
        auto fn = [parameters, body, &env](std::vector<LisppObject> arguments) {
                Environment local(std::make_shared<Environment>(env));
                if (arguments.size() != parameters.size()) {
                        throw invalid_arg_size("The procedure",
                                               arguments.size(),
                                               parameters.size());
                }
                for (size_t i = 0; i < parameters.size(); i++) {
                        auto parameter = parameters.at(i);
                        auto argument = arguments.at(i);
                        local.set(parameter.symbol, argument);
                }
                return evaluator::eval(body, local);
        };
        return LisppObject::create_function(fn);
}

bool is_self_evaluating(const LisppObject& ast)
{
        return ast.is_number() || ast.is_string() || ast.is_symbol();
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
        }
        else if (syntax::is_assigment(symbol)) {
                return eval_assignment(ast, env);
        }
        else if (syntax::is_local_assignment(symbol)) {
                return eval_local_assignment(ast, env);
        }
        else if (syntax::is_if(symbol)) {
                return eval_if(ast, env);
        }
        else if (syntax::is_function(symbol)) {
                return eval_function(ast, env);
        }
        else {
                LisppObject ast_value = eval_ast(ast, env);
                LisppObject function = syntax::apply_function(ast_value);
                std::vector<LisppObject> arguments =
                    syntax::apply_arguments(ast_value);
                return evaluator::apply(function, arguments);
        }
}

LisppObject evaluator::apply(const LisppObject& function,
                             const std::vector<LisppObject>& arguments)
{
        auto result = function.lambda(arguments);
        return result;
}
