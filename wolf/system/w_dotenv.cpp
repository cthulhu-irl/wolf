#include "wolf/system/w_dotenv.hpp"

#include <fstream>

#include "wolf/wolf.hpp"
#include "wolf/system/w_trace.hpp"

#include <ctre.hpp>

namespace wolf::system {

namespace internal {

static auto parse_error(std::string p_error)
{
    return W_FAILURE(std::errc::invalid_argument, std::move(p_error));
};

constexpr bool is_printable(char p_character)
{
    return 0x19 < p_character && p_character < 0x7f;
};

constexpr bool is_in_charset(const std::string_view p_charset, char p_character)
{
    return std::find(p_charset.begin(), p_charset.end(), p_character) != p_charset.end();
};

static boost::leaf::result<std::size_t> read_quoted_value_into(const std::string_view p_str,
                                                               std::size_t p_offset,
                                                               std::string& output)
{
    auto index = p_offset;
    auto size = p_str.size();

    if (index >= size) {
        return parse_error("internal: end of string has been reached already.");
    }

    if (p_str[index] != '\'' && p_str[index] != '"') {
        return parse_error("expected quotation.");
    }

    char ch_quote = p_str[index];
    ++index;

    bool is_escaping = false;
    while (index < size && is_printable(p_str[index])) {
        // prepare and skip the escape character, if not escaped itself
        if (!is_escaping && p_str[index] == '\\') {
            is_escaping = true;
            ++index;
            continue;
        }

        // if closing quote is not escaped, terminate
        if (!is_escaping && p_str[index] == ch_quote) {
            break;
        }

        // escape can be used only for certain characters
        if (is_escaping && !is_in_charset("`!$'\"\\", p_str[index])) {
            return parse_error("escape can only be used on `!$'\"\\ characters.");
        }

        is_escaping = false;

        output += p_str[index];
        ++index;
    }

    // ensure the quoted value ends with same quote character
    if (index >= size) {
        return parse_error("unexpected end of file/string.");
    }

    if (p_str[index] != ch_quote) {
        return parse_error("expected enclosing quote character.");
    }

    ++index;

    return index;
}

static boost::leaf::result<std::size_t> read_unquoted_value_into(const std::string_view p_str,
                                                                 std::size_t p_offset,
                                                                 std::string& output)
{
    constexpr auto must_escape_chars = "!$`'\"|\\";
    constexpr auto unescaped_punctuations = "~@#%^&*()-_=+:;,./?[]{}";

    const auto is_valid_char = [&](char character) {
        return std::isalnum(character)
               || is_in_charset(unescaped_punctuations, character)
               || is_in_charset(must_escape_chars, character);
    };

    auto index = p_offset;
    auto size = p_str.size();

    if (index >= size) {
        return parse_error("internal: end of string has been reached already.");
    }

    bool is_escaping = false;
    while (index < size && is_valid_char(p_str[index])) {
        // prepare and skip the escape character, if not escaped itself
        if (!is_escaping && p_str[index] == '\\') {
            is_escaping = true;
            ++index;
            continue;
        }

        // ensure must escape characters are preceded by escape character
        if (!is_escaping && is_in_charset(must_escape_chars, p_str[index])) {
            return parse_error(wolf::format("character was not escaped: {}", p_str[index]));
        }

        // only must escape characters can be escaped
        if (is_escaping && !is_in_charset(must_escape_chars, p_str[index])) {
            return parse_error(wolf::format("character must not be escaped: {}", p_str[index]));
        }

        is_escaping = false;

        output += p_str[index];
        ++index;
    }

    return index;
}

}  // namespace internal

boost::leaf::result<void> w_dotenv::load(const std::filesystem::path& p_path)
{
    auto file = std::ifstream(p_path);
    if (!file.is_open()) {
        return W_FAILURE(
            std::errc::invalid_argument,
            wolf::format("couldn't open given file: {}", p_path.string())
        );
    }

    std::string line;
    line.reserve(1024);

    while (std::getline(file, line)) {
        BOOST_LEAF_CHECK(load_from_string(line));
    }

    return {};
}

boost::leaf::result<void> w_dotenv::load_from_string(std::string_view p_str)
{
    std::size_t index = 0;
    const std::size_t size = p_str.size();

    // read one key=value on each iteration
    while (index < size) {
        std::string key;
        std::string value;

        // skip whitespaces
        while(index < size && std::isspace(p_str[index])) {
            ++index;
        }

        // if eos, return successfully
        if (index >= size) {
            break;
        }

        // if comment, skip the line
        if (p_str[index] == '#') {
            do {
                ++index;
            } while (index < size && p_str[index] != '\n');
            continue;
        }

        // there must be a key now,
        // starting with either alphabetic or underscore character
        if (p_str[index] != '_' && !std::isalpha(p_str[index])) {
            return internal::parse_error("expected start of key name with _ or alphabetic.");
        }
        do {
            key += p_str[index];
            ++index;
        } while (index < size && (p_str[index] == '_' || std::isalnum(p_str[index])));

        // there must be an equal sign after key variable name
        if (index >= size || p_str[index] != '=') {
            return internal::parse_error("after key name, there must be '=' character.");
        }
        ++index;

        // if eos or whitespace, it's a valid key assigned to be empty.
        if (index >= size || std::isspace(p_str[index])) {
            wolf::set_env(key, value);
            continue;
        }

        // if quoted value
        if (p_str[index] == '\'' || p_str[index] == '"') {
            BOOST_LEAF_ASSIGN(index, internal::read_quoted_value_into(p_str, index, value));
        } else { // reading in an unquoted value
            BOOST_LEAF_ASSIGN(index, internal::read_unquoted_value_into(p_str, index, value));
        }

        // ensure terminatation of key=value pair by either eos or whitespace.
        // this allows multiple keys being defined in the same line.
        if (index < size && !std::isspace(p_str[index])) {
            return internal::parse_error("after value, either file/string must end or whitespace.");
        }

        wolf::set_env(key, value);
    }

    return {};
}

}  // namespace wolf::system
