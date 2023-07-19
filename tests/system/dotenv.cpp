#include <boost/test/unit_test.hpp>

#include <iostream>

#include "wolf/wolf.hpp"
#include "wolf/system/w_leak_detector.hpp"
#include "wolf/system/w_dotenv.hpp"

using wolf::system::w_dotenv;
using wolf::system::w_leak_detector;

BOOST_AUTO_TEST_CASE(dotenv_parse) {
    std::cout << "entering test case 'dotenv_parse'" << std::endl;

    w_leak_detector _{};

    // valid cases
    BOOST_CHECK(w_dotenv::load_from_string(""));
    BOOST_CHECK(w_dotenv::load_from_string(" \t\n       \n \r\n \n"));
    BOOST_CHECK(w_dotenv::load_from_string("#"));
    BOOST_CHECK(w_dotenv::load_from_string("   #"));
    BOOST_CHECK(w_dotenv::load_from_string("#   "));
    BOOST_CHECK(w_dotenv::load_from_string("# some comment"));
    BOOST_CHECK(w_dotenv::load_from_string("###########"));

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_BASIC=foo"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_BASIC") == "foo");

    BOOST_CHECK(w_dotenv::load_from_string("  DOTENV_TEST_SPACE_BEFORE_KEY=foo"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_SPACE_BEFORE_KEY") == "foo");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_INTEGER_VALUE=123"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_INTEGER_VALUE") == "123");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_DECIMAL_VALUE=3.14"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_DECIMAL_VALUE") == "3.14");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_SINGLE_QUOTED='foo # bar'"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_SINGLE_QUOTED") == "foo # bar");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_DOUBLE_QUOTED=\"foo # bar\""));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_DOUBLE_QUOTED") == "foo # bar");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_MULTIPLE_EQ_SIGNS===foo"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_MULTIPLE_EQ_SIGNS") == "==foo");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_Valid_Key_1234=foo"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_Valid_Key_1234") == "foo");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_EMPTY="));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_EMPTY") == "");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_EMPTY_SINGLE_QUOTED=''"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_EMPTY_SINGLE_QUOTED") == "");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_EMPTY_DOUBLE_QUOTED=\"\""));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_EMPTY_DOUBLE_QUOTED") == "");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_COMMENT=foo # comment"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_COMMENT") == "foo");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_ENDING_IN_HASH=foo#bar"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_ENDING_IN_HASH") == "foo#bar");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_HASH_IN_BETWEEN=foo#bar"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_HASH_IN_BETWEEN") == "foo#bar");

    BOOST_CHECK(w_dotenv::load_from_string("DOTENV_TEST_INLINE_X=foo DOTENV_TEST_INLINE_Y=bar"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_INLINE_X") == "foo");
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_INLINE_Y") == "bar");

    constexpr auto input_escaped_value = R"str(\" \' \$ \! \` \\)str";
    constexpr auto output_escaped_value = R"str(" ' $ ! ` \)str";
    BOOST_CHECK(w_dotenv::load_from_string(
        wolf::format("DOTENV_TEST_ESCAPE_CHARS=\"{}\"", input_escaped_value)
    ));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_ESCAPE_CHARS") == output_escaped_value);

    constexpr auto input_value_chars = "abcdefghijklmnopqrstuvwxyz"
                                       "1234567890"
                                       "~`!@#$%^&*()-_=+|{}[]<>,./?:;'\\\\\\\"";
    constexpr auto output_value_chars = "abcdefghijklmnopqrstuvwxyz"
                                       "1234567890"
                                       "~`!@#$%^&*()-_=+|{}[]<>,./?:;'\\\"";
    BOOST_CHECK(w_dotenv::load_from_string(
        wolf::format("DOTENV_TEST_VALID_VALUE_CHARS=\"{}\"", input_value_chars)
    ));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_VALID_VALUE_CHARS") == output_value_chars);

    BOOST_CHECK(w_dotenv::load_from_string(R"(
        DOTENV_TEST_MULTIPLE_FOO=foo

        DOTENV_TEST_MULTIPLE_BAR='bar'
        DOTENV_TEST_MULTIPLE_BAZ="baz"
    )"));
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_MULTIPLE_FOO") == "foo");
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_MULTIPLE_BAR") == "bar");
    BOOST_CHECK(wolf::get_env("DOTENV_TEST_MULTIPLE_BAZ") == "baz");

    // invalid cases
    BOOST_CHECK(!w_dotenv::load_from_string("KEY_WITHOUT_EQ_SIGN"));
    BOOST_CHECK(!w_dotenv::load_from_string("1_STARTING_WITH_NUMBER"));
    BOOST_CHECK(!w_dotenv::load_from_string("SPACE_AFTER_KEY =foo"));
    BOOST_CHECK(!w_dotenv::load_from_string("SPACE_AFTER_EQ_SIGN= foo"));
    BOOST_CHECK(!w_dotenv::load_from_string("SPACE_IN_UNQUOTED_VALUE=foo bar"));
    BOOST_CHECK(!w_dotenv::load_from_string("UNCLOSED_SINGLE_QUOTE='foo"));
    BOOST_CHECK(!w_dotenv::load_from_string("UNCLOSED_DOUBLE_QUOTE=\"foo"));
    BOOST_CHECK(!w_dotenv::load_from_string("SINGLE_QUOTE_MISCLOSED='foo\""));
    BOOST_CHECK(!w_dotenv::load_from_string("DOUBLE_QUOTE_MISCLOSED=\"foo'"));
    BOOST_CHECK(!w_dotenv::load_from_string("Invalid-Key"));
    BOOST_CHECK(!w_dotenv::load_from_string("Invalid+Key"));
    BOOST_CHECK(!w_dotenv::load_from_string("Invalid Key=foo"));
    BOOST_CHECK(!w_dotenv::load_from_string("INVALID_UNQUOTED_VALUE=f\"oo"));

    std::cout << "exiting test case 'dotenv_parse'" << std::endl;
}

BOOST_AUTO_TEST_CASE(dotenv_from_file) {
    w_leak_detector _{};

    // valid cases
//    BOOST_CHECK(w_dotenv::load());
//    BOOST_CHECK(w_dotenv::load(".env.test"));

    // invalid cases
    BOOST_CHECK(!w_dotenv::load("nonexistent"));
}
