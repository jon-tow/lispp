#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "environment.h"
#include "evaluator.h"
#include "interpreter.h"

static Environment global_env{Environment::setup()};

// Keyword Operations Tests

// Assignment Tests
TEST_CASE("'def' Name Binding", "[def]")
{
        {
                auto binding = "(def hello \"hello world!\")";
                auto result = interpreter::REP(binding, global_env);
                auto expected = "hello world!";
                REQUIRE(result == expected);
        }
        {
                auto binding = "(def is_true true)";
                auto result = interpreter::REP(binding, global_env);
                auto expected = "true";
                REQUIRE(result == expected);
        }
        {
                auto binding = "(def is_false false)";
                auto result = interpreter::REP(binding, global_env);
                auto expected = "false";
                REQUIRE(result == expected);
        }
}

// Conditional Tests
TEST_CASE("'if' Conditional", "[if]")
{
        {
                auto condition = "(if (> 12 0) true false)";
                auto result = interpreter::REP(condition, global_env);
                auto expected = "true";
                REQUIRE(result == expected);
        }
        {
                auto condition = "(if (> 12 100) true false)";
                auto result = interpreter::REP(condition, global_env);
                auto expected = "false";
                REQUIRE(result == expected);
        }
}

// Arithmetic Tests
TEST_CASE("Arithmetic", "[arithmetic]")
{
        {
                auto sum = "(+ 12 -1)";
                auto result = interpreter::REP(sum, global_env);
                auto expected = "11.000000";
                REQUIRE(result == expected);
        }
}

// Comparisons Tests
TEST_CASE("Comparisons", "[comparator]")
{
        {
                auto comparison = "(< 3 2)";
                auto result = interpreter::REP(comparison, global_env);
                auto expected = "false";
                REQUIRE(result == expected);
        }
        {
                auto comparison = "(= 2 2)";
                auto result = interpreter::REP(comparison, global_env);
                auto expected = "true";
                REQUIRE(result == expected);
        }
}
