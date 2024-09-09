#include "TestTokenizer.hpp"
#include "gtest/gtest.h"

std::string get_kind_str(TokenKind kind) {
    switch (kind) {
        case None:
            return "None";
        case Char:
            return "Char";
        case Integer:
            return "Integer";
        case Decimal:
            return "Decimal";
        case TermPowSymbol:
            return "^";
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

TEST(TestTokenizer, TestIsChar) {

    for (int i = 0; i < 256; ++i) {
        std::string str = std::string(1, static_cast<char>(i));
        if (std::isalpha(i)) {
            EXPECT_TRUE(TestTokenizer::is_char(str));
        } else {
            EXPECT_FALSE(TestTokenizer::is_char(str));
        }
    }

    EXPECT_FALSE(TestTokenizer::is_char("a "));
    EXPECT_FALSE(TestTokenizer::is_char(" a"));
    EXPECT_FALSE(TestTokenizer::is_char(" a "));
    EXPECT_FALSE(TestTokenizer::is_char("aa"));
    EXPECT_FALSE(TestTokenizer::is_char("aaa"));
    EXPECT_FALSE(TestTokenizer::is_char("A123"));
    EXPECT_FALSE(TestTokenizer::is_char("1a"));
}

TEST(TestTokenizer, TestIsInteger) {
    for (int i = 0; i < 256; ++i) {
        std::string str = std::string(1, static_cast<char>(i));
        if (std::isdigit(i)) {
            EXPECT_TRUE(TestTokenizer::is_integer(str));
        } else {
            EXPECT_FALSE(TestTokenizer::is_integer(str));
        }
    }
    EXPECT_TRUE(TestTokenizer::is_integer("000"));
    EXPECT_TRUE(TestTokenizer::is_integer("000123"));
    EXPECT_TRUE(TestTokenizer::is_integer("123"));
    EXPECT_TRUE(TestTokenizer::is_integer("2147483647"));
    EXPECT_TRUE(TestTokenizer::is_integer("18446744073709551615"));
    EXPECT_TRUE(TestTokenizer::is_integer("99999999999999999999999999999999999999999999999"));

    EXPECT_FALSE(TestTokenizer::is_integer("nan"));
    EXPECT_FALSE(TestTokenizer::is_integer("inf"));
    EXPECT_FALSE(TestTokenizer::is_integer("+123"));
    EXPECT_FALSE(TestTokenizer::is_integer(" 123"));
    EXPECT_FALSE(TestTokenizer::is_integer("123abc"));
    EXPECT_FALSE(TestTokenizer::is_integer("123.456"));
    EXPECT_FALSE(TestTokenizer::is_integer("123 456"));
    EXPECT_FALSE(TestTokenizer::is_integer("123456 "));
}

TEST(TestTokenizer, TestIsDecimal) {
    for (int i = 0; i < 256; ++i) {
        std::string str = std::string(1, static_cast<char>(i));
        EXPECT_FALSE(TestTokenizer::is_decimal(str));
    }
    EXPECT_TRUE(TestTokenizer::is_decimal("0.0"));
    EXPECT_TRUE(TestTokenizer::is_decimal("00000.000"));
    EXPECT_TRUE(TestTokenizer::is_decimal("1.0"));
    EXPECT_TRUE(TestTokenizer::is_decimal("0001.123"));
    EXPECT_TRUE(TestTokenizer::is_decimal("0001.123"));
    EXPECT_TRUE(TestTokenizer::is_decimal("0.123"));

    EXPECT_FALSE(TestTokenizer::is_decimal(".0"));
    EXPECT_FALSE(TestTokenizer::is_decimal("0."));
    EXPECT_FALSE(TestTokenizer::is_decimal("0.123.456"));
    EXPECT_FALSE(TestTokenizer::is_decimal("0..123"));
    EXPECT_FALSE(TestTokenizer::is_decimal(" 0.123"));
    EXPECT_FALSE(TestTokenizer::is_decimal("0..123"));
    EXPECT_FALSE(TestTokenizer::is_decimal("0.123a"));
    EXPECT_FALSE(TestTokenizer::is_decimal("0a123a"));
    EXPECT_FALSE(TestTokenizer::is_decimal("1E+03"));
    EXPECT_FALSE(TestTokenizer::is_decimal("+123.456"));
    EXPECT_FALSE(TestTokenizer::is_decimal("123"));
    EXPECT_FALSE(TestTokenizer::is_decimal("123"));
    EXPECT_FALSE(TestTokenizer::is_decimal("inf"));
}

TEST(TestTokenizer, SplitByDelimiterString) {
    std::string equation;
    std::deque<std::string> expected_split, actual_split, src;
    char delimiter;
    bool keep_delimiter;

    delimiter = ' ';
    equation = "";
    expected_split = {};
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = '\0';
    equation = "";
    expected_split = {};
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = '\0';
    equation = " abc ";
    expected_split = {" abc "};
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = ' ';
    equation = "A";
    expected_split = {"A"};
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = ' ';
    equation = "A B C";
    //          ^ ^ ^
    expected_split = {"A", "B", "C"};
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = ' ';
    equation = "+A - B*C";
    //          ^^ ^ ^^^
    expected_split = {"+A", "-", "B*C"};
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = ' ';
    equation = "-1 * A^X + B^1-C ^X + D^+2";
    //          ^^ ^ ^^^ ^ ^^^ ^
    expected_split = {"-1", "*", "A^X", "+", "B^1-C", "^X", "+", "D^+2"};
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter);
    EXPECT_EQ(expected_split, actual_split);


    keep_delimiter = true;
    delimiter = '*';
    equation = "1*2*3";
    expected_split = {"1", "*", "2", "*", "3" };
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter, keep_delimiter);
    EXPECT_EQ(expected_split, actual_split);


    keep_delimiter = true;
    delimiter = '*';
    equation = "* 4*5* 666 *7ab*c **  ";
    //          ^^ ^ ^^^ ^ ^^^ ^
    expected_split = {
            "*", " 4", "*", "5", "*", " 666 ", "*", "7ab", "*", "c ", "*", "*", "  "
    };
    actual_split = TestTokenizer::split_by_delimiter(equation, delimiter, keep_delimiter);
    EXPECT_EQ(expected_split, actual_split);
}

TEST(TestTokenizer, SplitByDelimiterDeque) {
    std::deque<std::string> expected_split, actual_split, src;
    char delimiter;
    bool keep_delimiter = true;

    delimiter = '^';
    src = {"A^X", "+", "B^1-C"};
    //       ^           ^
    expected_split = {"A", "^", "X", "+", "B", "^", "1-C"};
    actual_split = TestTokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = '-';
    src = {"A^X", "-", "B^1-C"};
    //             ^       ^
    expected_split = {"A^X", "-", "B^1", "-", "C"};
    actual_split = TestTokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = ' ';
    src = {"  X^0 + X^1 +X^2    = 0   "};
    //      ^^   ^ ^   ^    ^^^^   ^^^
    expected_split = {"X^0", "+", "X^1", "+X^2", "=",  "0"};
    actual_split = TestTokenizer::split_by_delimiter(src, delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = '+';
    src = {"X^0", "+", "X^1", "+X^2", "=",  "0"};
    //             ^           ^
    expected_split = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    actual_split = TestTokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = '-';
    src = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    expected_split = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    actual_split = TestTokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(expected_split, actual_split);


    delimiter = '^';
    src = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    //       ^           ^           ^
    expected_split = {"X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "2", "=",  "0"};
    actual_split = TestTokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(expected_split, actual_split);
}


TEST(TestTokenizer, SplitEquation) {
    std::string equation;
    std::deque<std::string> expected_split, actual_split;

    equation = "";
    expected_split = {};
    actual_split = TestTokenizer::split_equation(equation);
    EXPECT_EQ(expected_split, actual_split);

    equation = "     ";
    expected_split = {};
    actual_split = TestTokenizer::split_equation(equation);
    EXPECT_EQ(expected_split, actual_split);


    equation = "1";
    expected_split = {"1"};
    actual_split = TestTokenizer::split_equation(equation);
    EXPECT_EQ(expected_split, actual_split);


    equation = "+-=*^";
    expected_split = {"+", "-", "=", "*", "^"};
    actual_split = TestTokenizer::split_equation(equation);
    EXPECT_EQ(expected_split, actual_split);


    equation = " 1 + 2 = 3 ";
    expected_split = {"1", "+", "2", "=", "3"};
    actual_split = TestTokenizer::split_equation(equation);
    EXPECT_EQ(expected_split, actual_split);


    equation = "  X^0 + X^1 +X^2    = 0   ";
    expected_split = {"X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "2", "=",  "0"};
    actual_split = TestTokenizer::split_equation(equation);
    EXPECT_EQ(expected_split, actual_split);


    equation = "  X ^ 0+X^  1 +X^+2    = 0  =0=++X^123  ";
    expected_split = { "X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "+", "2", "=", "0", "=", "0", "=", "+", "+", "X", "^", "123" };
    actual_split = TestTokenizer::split_equation(equation);
    EXPECT_EQ(expected_split, actual_split);
}



TEST(TestTokenizer, SplitCoefAndBase) {
    std::deque<s_token> tokens, actual_tokens, expected_tokens;


    tokens = {};
    expected_tokens = {};
    actual_tokens = TestTokenizer::split_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    tokens = {
            {.word="", .kind=None},
    };
    expected_tokens = {
            {.word="", .kind=None},
    };
    actual_tokens = TestTokenizer::split_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);



    tokens = {
            {.word="1X", .kind=None},
    };
    expected_tokens = {
            {.word="1", .kind=None},
            {.word="X", .kind=None},
    };
    actual_tokens = TestTokenizer::split_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    tokens = {
            {.word="1.02y", .kind=None},
    };
    expected_tokens = {
            {.word="1.02", .kind=None},
            {.word="y", .kind=None},
    };
    actual_tokens = TestTokenizer::split_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    tokens = {
            {.word="1.02xyz", .kind=None},
    };
    expected_tokens = {
            {.word="1.02", .kind=None},
            {.word="xyz", .kind=None},
    };
    actual_tokens = TestTokenizer::split_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    tokens = {
            {.word="1.02y", .kind=None},        // 1.02 / y
            {.word="1x", .kind=None},           // 1 / x
            {.word="x123", .kind=None},
            {.word="12.3.X123", .kind=None},
            {.word="12.3X123", .kind=None},
            {.word="x", .kind=Char},
    };
    expected_tokens = {
            {.word="1.02", .kind=None},
            {.word="y", .kind=None},
            {.word="1", .kind=None},
            {.word="x", .kind=None},
            {.word="x123", .kind=None},
            {.word="12.3.X123", .kind=None},
            {.word="12.3", .kind=None},
            {.word="X123", .kind=None},
            {.word="x", .kind=Char},
    };
    actual_tokens = TestTokenizer::split_coef_and_base(tokens);
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);
}


TEST(TestTokenizer, Tagging) {
    Tokenizer tokenizer;
    std::string equation;
    std::deque<s_token> actual_tokens, expected_tokens;

    equation = "";
    expected_tokens = {};
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "1";
    expected_tokens = {{.word="1", .kind=Integer}};
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "+ 1";
    expected_tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="1", .kind=Integer},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "1=2";
    expected_tokens = {
            {.word="1", .kind=Integer},
            {.word="=", .kind=OperatorEqual},
            {.word="2", .kind=Integer},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "  X^0 + X^1 +X^2    = 0   ";
    expected_tokens = {
            {.word="X", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=Integer},
            {.word="+", .kind=OperatorPlus},
            {.word="X", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="1", .kind=Integer},
            {.word="+", .kind=OperatorPlus},
            {.word="X", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="2", .kind=Integer},
            {.word="=", .kind=OperatorEqual},
            {.word="0", .kind=Integer},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();
    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);


    equation = "  1.50X^0 -2.3456X^1 +X^2.0    = 0.0   ";
    //               ^
    expected_tokens = {
            {.word="1.50", .kind=Decimal},
            {.word="X", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=Integer},
            {.word="-", .kind=OperatorMinus},
            {.word="2.3456", .kind=Decimal},
            {.word="X", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="1", .kind=Integer},
            {.word="+", .kind=OperatorPlus},
            {.word="X", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="2.0", .kind=Decimal},
            {.word="=", .kind=OperatorEqual},
            {.word="0.0", .kind=Decimal},
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
            {.word="X", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=Integer},
            {.word="^", .kind=TermPowSymbol},
            {.word="1", .kind=Integer},
            {.word="=", .kind=OperatorEqual},
            {.word="=", .kind=OperatorEqual},
            {.word="0", .kind=Integer},
            {.word="^", .kind=TermPowSymbol},
            {.word="+", .kind=OperatorPlus},
            {.word="1", .kind=Integer},
            {.word="*", .kind=OperatorMul},
    };
    tokenizer.tokenize(equation);
    actual_tokens = tokenizer.tokens();

    // for (auto &token: actual_tokens) {
    //     std::cout << token.word << std::endl;
    // }

    expect_eq_tokens(expected_tokens, actual_tokens, __LINE__);

}
