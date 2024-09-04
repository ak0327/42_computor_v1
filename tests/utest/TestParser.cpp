#include "Parser.hpp"
#include "gtest/gtest.h"

TEST(TestParser, TestSkipSP) {
    std::string str;
    std::size_t start, actual_end, expected_end;

    str = " 1";
    //     01234
    //   st^^end
    start = 0;
    expected_end = 1;
    Parser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "   1";
    //     01234
    //   st^  ^end
    start = 0;
    expected_end = 3;
    Parser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "   1";
    //     01234
    //   st^  ^end
    start = 0;
    expected_end = 3;
    Parser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "";
    //     01234
    //   st^end
    start = 0;
    expected_end = start;
    Parser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "";
    //     01234
    //       st^end
    start = 4;
    expected_end = start;
    Parser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "abc";
    //     01234
    //   st^end
    start = 0;
    expected_end = start;
    Parser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
}

//  term = ( sign ) ( coefficient "*" ) ALPHA "^" 1*( DIGIT )
//         ^^^^^^^^^^^^^^^^^^^^^^ stoi
TEST(TestParser, TestStoiOK) {
    std::string str;
    std::size_t start, actual_end, expected_end;
    int actual_num, expected_num;

    expected_num = 1;
    str = "1";
    //     01234
    //      ^end
    expected_end = 1;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = 1;
    str = " 1";
    //     01234
    //       ^end
    expected_end = 2;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = -1;
    str = " -000001     ";
    //     0123456789012345
    //             ^end
    expected_end = 8;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = 1;
    str = " + 01     ";
    //     0123456789012345
    //          ^end
    expected_end = 5;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = 123;
    str = " 123abc";
    //     012345678
    //         ^end
    expected_end = 4;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = 123;
    str = "abc123abc";
    //     012345678
    //      st^  ^end
    expected_end = 6;
    start = 3;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = 0;
    str = "0";
    //     01234
    //      ^end
    expected_end = 1;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = 0;
    str = " -0000  ";
    //     012345678
    //           ^end
    expected_end = 6;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = 2147483647;
    str = "2147483647";
    //     0123456789012345
    //               ^end
    expected_end = 10;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = 2147483647;
    str = " + 0000002147483647   ";
    //     01234567890123456789012345
    //                        ^end
    expected_end = 19;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = -2147483648;
    str = "-0000002147483648";
    //     01234567890123456789012345
    //                      ^end
    expected_end = 17;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);


    expected_num = -2147483648;
    str = "-  2147483648   ";
    //     01234567890123456789012345
    //                  ^end
    expected_end = 13;
    start = 0;
    actual_num = Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_num, actual_num);
    EXPECT_EQ(expected_end, actual_end);
}


TEST(TestParser, TestStoiNG) {
    std::string str;
    std::size_t start, actual_end, expected_end;

    str = "abc";
    //     01234
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = " + +1";
    //     01234
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = "2147483648";  // out of range
    //     01234567890
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = "4294967295";  // out of range
    //     01234567890
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = "18446744073709551615";  // out of range
    //     01234567890
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = "-2147483649";  // out of range
    //     01234567890
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = "-9223372036854775808";  // out of range
    //     01234567890
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = "-18446744073709551615";  // out of range
    //     01234567890
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = "-18446744073709551615";  // out of range
    //     01234567890
    //       st^
    //         ^end
    start = 4;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = "";
    //     01234567890
    //     ^end
    start = 0;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "";
    //     01234567890
    //     ^end
    start = 3;
    expected_end = start;
    Parser::stoi(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
}


TEST(TestParser, TestParseTermOK) {
    std::string expr;
    std::size_t start, actual_end, expected_end;
    s_term actual_term, expected_term;

    expected_term.coefficient = 1;
    expected_term.variable = 'X';
    expected_term.degree = 1;
    expr = "X^1";
    //      01234
    //    st^  ^end
    start = 0;
    expected_end = 3;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expected_term.coefficient = 2;
    expected_term.variable = 'X';
    expected_term.degree = 0;
    expr = " 2 * X^0 ";
    //      01234567890
    //    st^        ^end
    start = 0;
    expected_end = 9;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expected_term.coefficient = 2;
    expected_term.variable = 'X';
    expected_term.degree = 0;
    expr = " 2 * X^0 + 3";
    //      01234567890
    //    st^        ^end
    start = 0;
    expected_end = 9;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expected_term.coefficient = 20;
    expected_term.variable = 'a';
    expected_term.degree = 1;
    expr = " +020*a^01 - 3";
    //      0123456789012345
    //    st^          ^end
    start = 0;
    expected_end = 11;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);
}


TEST(TestParser, TestParseTermNG) {
    std::string expr;
    std::size_t start, actual_end, expected_end;
    s_term actual_term, expected_term;

    expected_term.coefficient = 0;
    expected_term.variable = '\0';
    expected_term.degree = 0;
    expr = "XY^1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "X^+1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "X^-1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "10.1 * X^1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "1X^1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "1*1*X^1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "1^1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);

    expr = "";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "";
    //      01234567890
    //    st^end
    start = 10;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "X*X^1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "X^ 1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "X ^1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "X^";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "X^ ";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    expr = "X^ X^1";
    //      01234567890
    //    st^end
    start = 0;
    expected_end = start;
    actual_term = Parser::parse_term(expr, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.degree, actual_term.degree);
}

// TEST(TestParser, ParseOK) {
//     Parser parser;
//     std::string equation;
//     Status ret;
//
//     // simple equation
//     equation = "X^0 + X^1 + X^2 = 0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::SUCCESS, ret);
//
//     equation = "  X^0 + X^1 + X^2    = 0   ";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::SUCCESS, ret);
//
//     equation = "x^0 + x^1 + x^2 = 0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::SUCCESS, ret);
//
//     equation = "A^0 + A^1 + A^2 = 0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::SUCCESS, ret);
//
//     equation = "0 = X^0 + X^1 + X^2";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::SUCCESS, ret);
//
//     // subject
//     equation = "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::SUCCESS, ret);
//
//     equation = "5 * X^0 + 4 * X^1 = 4 * X^0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::SUCCESS, ret);
//
//     equation = "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::SUCCESS, ret);
// }
//
// TEST(TestParser, ParseNG) {
//     Parser parser;
//     std::string equation;
//     Status ret;
//
//     equation = "X^0 = 0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::FAILURE, ret);
//
//     equation = "X^0 == 0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::FAILURE, ret);
//
//     equation = "X^0 = 0 = 0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::FAILURE, ret);
//
//     equation = "= X^0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::FAILURE, ret);
//
//     equation = "X^0 =";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::FAILURE, ret);
//
//     equation = "X^0 + Y^0";
//     ret = parser.parse_equation(equation);
//     EXPECT_EQ(Status::FAILURE, ret);
// }
//
// TEST(TestParser, ValidateOK) {
//     Parser parser;
//     std::string equation;
//     Status ret;
//
//     // simple equation
//     equation = "X^2 = 0";
//     parser.parse_equation(equation);
//     ret = parser.validate_polynomial();
//     EXPECT_EQ(Status::SUCCESS, ret);
//
//
// }
//
// TEST(TestParser, ValidateNG) {
//     Parser parser;
//     std::string equation;
//     Status ret;
//
//     // simple equation
//     equation = "X^3 = 0";
//     parser.parse_equation(equation);
//     ret = parser.validate_polynomial();
//     EXPECT_EQ(Status::FAILURE, ret);
// }
