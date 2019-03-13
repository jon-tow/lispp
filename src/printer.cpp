#include "printer.h"

using namespace type;

namespace {

std::string ast_to_string(const LisppObject &ast)
{
        std::string result = "";
        std::string padding = " ";
        switch (ast.type) {
        case Type::String:
                result = ast.string;
                break;
        case Type::Number:
                result = std::to_string(ast.number);
                break;
        case Type::Symbol:
                result = ast.symbol;
                break;
        case Type::True:
                result = "true";
                break;
        case Type::False:
                result = "false";
                break;
        case Type::Nil:
                result = "nil";
                break;
        case Type::Function:
                result = "#<function>";
                break;
        case Type::List:
                result += "(";
                for (auto i = ast.items.begin(); i != ast.items.end(); ++i) {
                        if (std::next(i) == ast.items.end()) {
                                padding = "";
                        }
                        result += ast_to_string(*i) + padding;
                }
                result += ")";
                break;
        }
        return result;
}

} // namespace

void printer::prompt()
{
        std::string prompt = "]=> ";
        std::cout << prompt;
}

void printer::welcome()
{
        std::string welcome = "\nWelcome to Lispp.\nA LISP in C++.\n";
        std::cout << welcome << std::endl;
}

void printer::print(const LisppObject &ast)
{
        std::string value_prompt = ";Value: ";
        std::string ast_output = ast_to_string(ast);
        std::string output = "\n" + value_prompt + ast_output + "\n";
        std::cout << output << std::endl;
}
