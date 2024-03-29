#include "reader.h"

using namespace type;

namespace {

std::optional<double> token_to_number(const std::string& token)
{
        // TODO: Make `number` double-type when GCC/Clang adds support for
        // floating-point `std::from_chars`. This will make `LisppObject.number`
        // floating-point compatible.
        // double number = 0.0;
        // auto data = token.data();
        // auto size = token.size();
        // auto [p, ec] = std::from_chars(data, data + size, number);
        // if (ec != std::errc()) {
        //         return std::nullopt;
        // }
        // Temporary Hack.
        std::optional<double> number = std::nullopt;
        try {
                number = std::stod(token.c_str());
        }
        catch (const std::invalid_argument& ia) {
                /// Ignore the exception.
        }
        return number;
}

std::string clean(const std::string& text)
{
        std::string clean{text};
        clean.erase(std::remove(clean.begin(), clean.end(), ','), clean.end());
        return clean;
}

std::vector<std::string> split(const std::string& text,
                               const std::string regex_str)
{
        std::regex regex{regex_str};
        std::sregex_token_iterator iter(text.begin(), text.end(), regex);
        std::sregex_token_iterator end;
        std::vector<std::string> split;
        for (; iter != end; iter++) {
                split.push_back(*iter);
        }
        return split;
}

} // namespace

// Parse a `program` into `Lispp` internal representation.
LisppObject Reader::read(const std::string& program)
{
        auto tokens = tokenize(program);
        auto reader = Reader(tokens);
        auto form = reader.read_form();
        return form;
}

// Simple lexical analysis based on splitting.
std::vector<std::string> Reader::tokenize(const std::string& text)
{
        std::string cleansed_text{clean(text)};
        auto tokens = split(cleansed_text, syntax::grammar);
        return tokens;
}

// Read a `Lispp` expression from the internal tokenized string.
LisppObject Reader::read_form()
{
        if (tokens.empty()) {
                throw std::runtime_error("\n;Unexpected EOF.\n");
        }
        std::string token = peek().value_or("");
        if (syntax::is_list_delimited(token)) {
                return read_list();
        }
        else if (syntax::is_string_delimited(token)) {
                return read_string();
        }
        else {
                return read_atom();
        }
}

LisppObject Reader::read_list()
{
        auto list = LisppObject::create_list({});
        while (next().has_value() && peek().value_or(")") != ")") {
                auto form = read_form();
                list.items.push_back(form);
        }
        if (!peek().has_value()) {
                /* Note: Remove after allowing <enter>
                 *       presses until user balances parens.
                 */
                throw std::runtime_error("\n;Unbalanced parentheses.\n");
        }
        return list;
}

LisppObject Reader::read_string()
{
        std::string str = peek().value_or("");
        while (peek().value().back() != '"' && next().has_value()) {
                str += " " + peek().value();
        }
        if (str.back() != '"') {
                /* Note: Remove after allowing <enter>
                 *       presses until user balances string.
                 */
                throw std::runtime_error("\n;Unbalanced string.\n");
        }
        str = str.substr(1, str.length() - 2); // Remove string quotes: ""
        return LisppObject::create_string(str);
}

LisppObject Reader::read_atom()
{
        std::string token = peek().value_or("");
        if (token_to_number(token) != std::nullopt) {
                auto num = token_to_number(token).value();
                return LisppObject::create_number(num);
        }
        else if (token == type::types[Type::True]) {
                return LisppObject::create_true();
        }
        else if (token == type::types[Type::False]) {
                return LisppObject::create_false();
        }
        else if (token == type::types[Type::Nil]) {
                return LisppObject::create_nil();
        }
        else {
                return LisppObject::create_symbol(token);
        }
}

std::optional<std::string> Reader::next()
{
        if (out_of_bounds()) {
                return std::nullopt;
        }
        return tokens.at(position++);
}

std::optional<std::string> Reader::peek()
{
        if (out_of_bounds()) {
                return std::nullopt;
        }
        return tokens.at(position);
}

bool Reader::out_of_bounds() { return position >= tokens.size(); }
