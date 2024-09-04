#include "Parser.hpp"
#include "gtest/gtest.h"

TEST(TestParser, ParseOK) {
    Parser parser;
    std::string equation;
    Status ret;

    // simple equation
    equation = "X^0 + X^1 + X^2 = 0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::SUCCESS, ret);

    equation = "  X^0 + X^1 + X^2    = 0   ";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::SUCCESS, ret);

    equation = "x^0 + x^1 + x^2 = 0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::SUCCESS, ret);

    equation = "A^0 + A^1 + A^2 = 0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::SUCCESS, ret);

    equation = "0 = X^0 + X^1 + X^2";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::SUCCESS, ret);

    // subject
    equation = "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::SUCCESS, ret);

    equation = "5 * X^0 + 4 * X^1 = 4 * X^0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::SUCCESS, ret);

    equation = "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::SUCCESS, ret);
}

TEST(TestParser, ParseNG) {
    Parser parser;
    std::string equation;
    Status ret;

    equation = "X^0 = 0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::FAILURE, ret);

    equation = "X^0 == 0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::FAILURE, ret);

    equation = "X^0 = 0 = 0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::FAILURE, ret);

    equation = "= X^0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::FAILURE, ret);

    equation = "X^0 =";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::FAILURE, ret);

    equation = "X^0 + Y^0";
    ret = parser.parse_equation(equation);
    EXPECT_EQ(Status::FAILURE, ret);
}

TEST(TestParser, ValidateOK) {
    Parser parser;
    std::string equation;
    Status ret;

    // simple equation
    equation = "X^2 = 0";
    parser.parse_equation(equation);
    ret = parser.validate_polynomial();
    EXPECT_EQ(Status::SUCCESS, ret);


}

TEST(TestParser, ValidateNG) {
    Parser parser;
    std::string equation;
    Status ret;

    // simple equation
    equation = "X^3 = 0";
    parser.parse_equation(equation);
    ret = parser.validate_polynomial();
    EXPECT_EQ(Status::FAILURE, ret);
}
