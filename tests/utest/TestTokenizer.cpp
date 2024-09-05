#include "Tokenizer.hpp"
#include "gtest/gtest.h"

std::string get_kind_str(TokenKind kind) {
    switch (kind) {
        case None:
            return "None";
        case TermCoef:
            return "TermCoef";
        case TermBase:
            return "TermBase";
        case TermPowSymbol:
            return "^";
        case TermPower:
            return "TermPower";
        case OperatorPlus:
            return "+";
        case OperatorMinus:
            return "-";
        case OperatorMul:
            return "*";
        case OperatorEqual:
            return "=";
        default:
            return "Error";
    }
}

void expect_eq_tokens(
        const std::deque<s_token> &expected,
        const std::deque<s_token> &actual,
        std::size_t line) {
    EXPECT_EQ(expected.size(), actual.size()) << " at L:" << line << std::endl;
    if (expected.size() != actual.size()) {
        FAIL();
    }
    for (std::size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(expected[i].word, actual[i].word) << " expected: " << expected[i].word << " but actual: " << actual[i].word << " at L:" << line << std::endl;
        EXPECT_EQ(expected[i].kind, actual[i].kind) << " [" << actual[i].word <<  "] expected: " << get_kind_str(expected[i].kind) << " but actual: " << get_kind_str(actual[i].kind) << "  at L:" << line << std::endl;
    }
}


TEST(TestParser, SplitByDelimiterString) {
    std::string equation;
    std::deque<std::string> actual_split, expected_split, src;
    char delimiter;
    bool keep_delimiter;

    delimiter = ' ';
    equation = "";
    expected_split = {};
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '\0';
    equation = "";
    expected_split = {};
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '\0';
    equation = " abc ";
    expected_split = {" abc "};
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    equation = "A";
    expected_split = {"A"};
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    equation = "A B C";
    //          ^ ^ ^
    expected_split = {"A", "B", "C"};
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    equation = "+A - B*C";
    //          ^^ ^ ^^^
    expected_split = {"+A", "-", "B*C"};
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    equation = "-1 * A^X + B^1-C ^X + D^+2";
    //          ^^ ^ ^^^ ^ ^^^ ^
    expected_split = {"-1", "*", "A^X", "+", "B^1-C", "^X", "+", "D^+2"};
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    keep_delimiter = true;
    delimiter = '*';
    equation = "1*2*3";
    expected_split = {"1", "*", "2", "*", "3" };
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    keep_delimiter = true;
    delimiter = '*';
    equation = "* 4*5* 666 *7ab*c **  ";
    //          ^^ ^ ^^^ ^ ^^^ ^
    expected_split = {
            "*", " 4", "*", "5", "*", " 666 ", "*", "7ab", "*", "c ", "*", "*", "  "
    };
    actual_split = Tokenizer::split_by_delimiter(equation, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);
}

TEST(TestParser, SplitByDelimiterDeque) {
    std::deque<std::string> actual_split, expected_split, src;
    char delimiter;
    bool keep_delimiter = true;

    delimiter = '^';
    src = {"A^X", "+", "B^1-C"};
    //       ^           ^
    expected_split = {"A", "^", "X", "+", "B", "^", "1-C"};
    actual_split = Tokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '-';
    src = {"A^X", "-", "B^1-C"};
    //             ^       ^
    expected_split = {"A^X", "-", "B^1", "-", "C"};
    actual_split = Tokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    src = {"  X^0 + X^1 +X^2    = 0   "};
    //      ^^   ^ ^   ^    ^^^^   ^^^
    expected_split = {"X^0", "+", "X^1", "+X^2", "=",  "0"};
    actual_split = Tokenizer::split_by_delimiter(src, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '+';
    src = {"X^0", "+", "X^1", "+X^2", "=",  "0"};
    //             ^           ^
    expected_split = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    actual_split = Tokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '-';
    src = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    expected_split = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    actual_split = Tokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '^';
    src = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    //       ^           ^           ^
    expected_split = {"X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "2", "=",  "0"};
    actual_split = Tokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);
}


TEST(TestParser, SplitEquation) {
    std::string equation;
    std::deque<std::string> actual_split, expected_split;

    equation = "";
    expected_split = {};
    actual_split = Tokenizer::split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);

    equation = "     ";
    expected_split = {};
    actual_split = Tokenizer::split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);


    equation = "1";
    expected_split = {"1"};
    actual_split = Tokenizer::split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);


    equation = "+-=*^";
    expected_split = {"+", "-", "=", "*", "^"};
    actual_split = Tokenizer::split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);


    equation = " 1 + 2 = 3 ";
    expected_split = {"1", "+", "2", "=", "3"};
    actual_split = Tokenizer::split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);


    equation = "  X^0 + X^1 +X^2    = 0   ";
    expected_split = {"X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "2", "=",  "0"};
    actual_split = Tokenizer::split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);


    equation = "  X ^ 0+X^  1 +X^+2    = 0  =0=++X^123  ";
    expected_split = { "X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "+", "2", "=", "0", "=", "0", "=", "+", "+", "X", "^", "123" };
    actual_split = Tokenizer::split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);
}



TEST(TestParser, SplitCoefAndBase) {
    std::deque<s_token> tokens, actual_tokens, expected_tokens;


    tokens = {};
    expected_tokens = {};
    actual_tokens = Tokenizer::split_term_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    tokens = {
            {.word="", .kind=None},
    };
    expected_tokens = {
            {.word="", .kind=None},
    };
    actual_tokens = Tokenizer::split_term_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);



    tokens = {
            {.word="1X", .kind=None},
    };
    expected_tokens = {
            {.word="1", .kind=None},
            {.word="X", .kind=None},
    };
    actual_tokens = Tokenizer::split_term_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    tokens = {
            {.word="1.02y", .kind=None},
    };
    expected_tokens = {
            {.word="1.02", .kind=None},
            {.word="y", .kind=None},
    };
    actual_tokens = Tokenizer::split_term_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    tokens = {
            {.word="1.02xyz", .kind=None},
    };
    expected_tokens = {
            {.word="1.02", .kind=None},
            {.word="xyz", .kind=None},
    };
    actual_tokens = Tokenizer::split_term_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    tokens = {
            {.word="1.02y", .kind=None},        // 1.02 / y
            {.word="1x", .kind=None},           // 1 / x
            {.word="x123", .kind=None},
            {.word="12.3.X123", .kind=None},    // 12.3. / X123
            {.word="1x", .kind=TermCoef},
    };
    expected_tokens = {
            {.word="1.02", .kind=None},
            {.word="y", .kind=None},
            {.word="1", .kind=None},
            {.word="x", .kind=None},
            {.word="x123", .kind=None},
            {.word="12.3.", .kind=None},
            {.word="X123", .kind=None},
            {.word="1x", .kind=TermCoef},
    };
    actual_tokens = Tokenizer::split_term_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);
}


TEST(TestParser, Tagging) {
    Tokenizer tokenizer;
    std::string equation;
    std::deque<s_token> actual_tokens, expected_tokens;

    equation = "";
    expected_tokens = {};
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "1";
    expected_tokens = {{.word="1", .kind=TermCoef}};
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "+ 1";
    expected_tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="1", .kind=TermCoef},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "1=2";
    expected_tokens = {
            {.word="1", .kind=TermCoef},
            {.word="=", .kind=OperatorEqual},
            {.word="2", .kind=TermCoef},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "  X^0 + X^1 +X^2    = 0   ";
    expected_tokens = {
            {.word="X", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=TermPower},
            {.word="+", .kind=OperatorPlus},
            {.word="X", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="1", .kind=TermPower},
            {.word="+", .kind=OperatorPlus},
            {.word="X", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="2", .kind=TermPower},
            {.word="=", .kind=OperatorEqual},
            {.word="0", .kind=TermCoef},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "  1.50X^0 -2.3456X^1 +X^2.0    = 0.0   ";
    //               ^
    expected_tokens = {
            {.word="1.50", .kind=TermCoef},
            {.word="X", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=TermPower},
            {.word="-", .kind=OperatorMinus},
            {.word="2.3456", .kind=TermCoef},
            {.word="X", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="1", .kind=TermPower},
            {.word="+", .kind=OperatorPlus},
            {.word="X", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="2.0", .kind=TermPower},
            {.word="=", .kind=OperatorEqual},
            {.word="0.0", .kind=TermCoef},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();

    // for (auto &token: actual_tokens) {
    //     std::cout << token.word << std::endl;
    // }

    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "= -X^0^1 == 0^+1*";
    expected_tokens = {
            {.word="=", .kind=OperatorEqual},
            {.word="-", .kind=OperatorMinus},
            {.word="X", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=None},
            {.word="^", .kind=TermPowSymbol},
            {.word="1", .kind=TermPower},
            {.word="=", .kind=OperatorEqual},
            {.word="=", .kind=OperatorEqual},
            {.word="0", .kind=None},
            {.word="^", .kind=TermPowSymbol},
            {.word="+", .kind=OperatorPlus},
            {.word="1", .kind=TermCoef},
            {.word="*", .kind=OperatorMul},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();

    // for (auto &token: actual_tokens) {
    //     std::cout << token.word << std::endl;
    // }

    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);

}
