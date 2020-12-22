#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "evaluator.h"
#include "frame.h"
#include "interpreter.h"

// Keyword Operations Tests

// Assignment Tests
TEST_CASE("'def' Name Binding", "[def]")
{
        Frame global_frame;
        {
                auto binding = "(def hello \"hello world!\")";
                auto result = interpreter::rep(binding, global_frame);
                auto expected = "hello world!";
                REQUIRE(result == expected);
        }
        {
                auto binding = "(def is_true true)";
                auto result = interpreter::rep(binding, global_frame);
                auto expected = "true";
                REQUIRE(result == expected);
        }
        {
                auto binding = "(def is_false false)";
                auto result = interpreter::rep(binding, global_frame);
                auto expected = "false";
                REQUIRE(result == expected);
        }
}

// Conditional Tests
TEST_CASE("'if' Conditional", "[if]")
{
        Frame global_frame{Frame::global()};
        {
                auto condition = "(if (> 12 0) true false)";
                auto result = interpreter::rep(condition, global_frame);
                auto expected = "true";
                REQUIRE(result == expected);
        }
        {
                auto condition = "(if (> 12 100) true false)";
                auto result = interpreter::rep(condition, global_frame);
                auto expected = "false";
                REQUIRE(result == expected);
        }
}

// Arithmetic Tests
TEST_CASE("Arithmetic", "[arithmetic]")
{
        Frame global_frame{Frame::global()};
        {
                auto sum = "(+ 12 -1)";
                auto result = interpreter::rep(sum, global_frame);
                auto expected = "11.000000";
                REQUIRE(result == expected);
        }
}

// Relational Tests
TEST_CASE("Relational", "[comparator]")
{
        Frame global_frame{Frame::global()};
        {
                auto comparison = "(< 3 2)";
                auto result = interpreter::rep(comparison, global_frame);
                auto expected = "false";
                REQUIRE(result == expected);
        }
        {
                auto comparison = "(= 2 2)";
                auto result = interpreter::rep(comparison, global_frame);
                auto expected = "true";
                REQUIRE(result == expected);
        }
}
