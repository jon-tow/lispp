#include "reader.h"

using namespace type;

namespace {

std::optional<int> string_to_number(const std::string &text)
{
        auto number = 0;
        auto data = text.data();
        auto sz = text.size();
        auto [p, ec] = std::from_chars(data, data + sz, number);
        if (ec != std::errc()) {
                return std::nullopt;
        }
        return number;
}

std::string replace(const std::string &text, const std::string &old,
                    const std::string &update)
{
        std::string str(text);
        auto pos = str.find(old);
        while (pos != std::string::npos) {
                str = str.replace(pos, old.length(), update);
                pos = str.find(old, pos + update.length());
        }
        return str;
}

std::string clean(const std::string &text)
{
        std::string pad(text);
        pad = replace(pad, "(", " ( ");
        pad = replace(pad, ")", " ) ");
        pad.erase(std::remove(pad.begin(), pad.end(), ','), pad.end());
        return pad;
}

std::vector<std::string> split(const std::string &text)
{
        std::istringstream buff(text);
        std::vector<std::string> split(
            (std::istream_iterator<std::string>(buff)),
            std::istream_iterator<std::string>());
        return split;
}

} // namespace

LisppObject Reader::read(const std::string &program)
{
        auto tokens = tokenize(program);
        auto reader = Reader(tokens);
        return reader.read_form();
}

std::vector<std::string> Reader::tokenize(const std::string &text)
{
        std::string cleaned = clean(text);
        auto tokens = split(cleaned);
        return tokens;
}

LisppObject Reader::read_form()
{
        std::string token = peek().value_or("");
        if (token == "(") {
                return read_list();
        }
        return read_atom();
}

LisppObject Reader::read_list()
{
        auto list = LisppObject::create_list({});
        while (next().has_value() && peek().value_or("") != ")") {
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

LisppObject Reader::read_atom()
{
        std::string token = peek().value_or("");
        if (string_to_number(token) != std::nullopt) {
                auto num = string_to_number(token).value();
                return LisppObject::create_number(num);
        }
        else if (token == syntax::types[Type::True]) {
                return LisppObject::create_true();
        }
        else if (token == syntax::types[Type::False]) {
                return LisppObject::create_false();
        }
        else if (token == syntax::types[Type::Nil]) {
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
