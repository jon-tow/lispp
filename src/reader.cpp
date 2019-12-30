#include "reader.h"

using namespace type;

namespace {

bool is_list_delimited(const std::string& token) { return token == "("; }

bool is_string_delimited(const std::string& token)
{
        return token.at(0) == '"';
}

std::optional<int> tokens_to_number(const std::string& token)
{
        // TODO: Make `number` double-type when GCC/Clang adds support for
        // floating-point `std::from_chars`. This will make `LisppObject.number`
        // floating-point compatible.
        auto number = 0;
        auto data = token.data();
        auto size = token.size();
        auto [p, ec] = std::from_chars(data, data + size, number);
        if (ec != std::errc()) {
                return std::nullopt;
        }
        return number;
}

std::string clean(const std::string& text)
{
        std::string clean(text);
        clean.erase(std::remove(clean.begin(), clean.end(), ','), clean.end());
        return clean;
}

std::vector<std::string> split(const std::string& text,
                               const std::string regex_str)
{
        std::vector<std::string> split;
        std::regex regex(regex_str);
        std::sregex_token_iterator iter(text.begin(), text.end(), regex);
        std::sregex_token_iterator end;
        for (; iter != end; iter++) {
                split.push_back(*iter);
        }
        return split;
}

} // namespace

LisppObject Reader::read(const std::string& program)
{
        auto tokens = tokenize(program);
        auto reader = Reader(tokens);
        auto form = reader.read_form();
        return form;
}

std::vector<std::string> Reader::tokenize(const std::string& text)
{
        std::string cleansed_text = clean(text);
        auto tokens = split(cleansed_text, syntax::grammar);
        return tokens;
}

LisppObject Reader::read_form()
{
        std::string token = peek().value_or("");
        if (is_list_delimited(token)) {
                return read_list();
        }
        else if (is_string_delimited(token)) {
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
        str = str.substr(1, str.length() - 2);
        return LisppObject::create_string(str);
}

LisppObject Reader::read_atom()
{
        std::string token = peek().value_or("");
        if (tokens_to_number(token) != std::nullopt) {
                auto num = tokens_to_number(token).value();
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
