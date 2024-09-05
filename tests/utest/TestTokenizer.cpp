#include "Tokenizer.hpp"
#include "gtest/gtest.h"

TEST(TestParser, SplitByDelimiterString) {
    Tokenizer tokenizer;
    std::string equation;
    std::deque<std::string> actual_split, expected_split, src;
    char delimiter;
    bool keep_delimiter;

    delimiter = ' ';
    equation = "";
    expected_split = {};
    actual_split = tokenizer.split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '\0';
    equation = "";
    expected_split = {};
    actual_split = tokenizer.split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '\0';
    equation = " abc ";
    expected_split = {" abc "};
    actual_split = tokenizer.split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    equation = "A";
    expected_split = {"A"};
    actual_split = tokenizer.split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    equation = "A B C";
    //          ^ ^ ^
    expected_split = {"A", "B", "C"};
    actual_split = tokenizer.split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    equation = "+A - B*C";
    //          ^^ ^ ^^^
    expected_split = {"+A", "-", "B*C"};
    actual_split = tokenizer.split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    equation = "-1 * A^X + B^1-C ^X + D^+2";
    //          ^^ ^ ^^^ ^ ^^^ ^
    expected_split = {"-1", "*", "A^X", "+", "B^1-C", "^X", "+", "D^+2"};
    actual_split = tokenizer.split_by_delimiter(equation, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    keep_delimiter = true;
    delimiter = '*';
    equation = "1*2*3";
    expected_split = {"1", "*", "2", "*", "3" };
    actual_split = tokenizer.split_by_delimiter(equation, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    keep_delimiter = true;
    delimiter = '*';
    equation = "* 4*5* 666 *7ab*c **  ";
    //          ^^ ^ ^^^ ^ ^^^ ^
    expected_split = {
            "*", " 4", "*", "5", "*", " 666 ", "*", "7ab", "*", "c ", "*", "*", "  "
    };
    actual_split = tokenizer.split_by_delimiter(equation, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);
}

TEST(TestParser, SplitByDelimiterDeque) {
    Tokenizer tokenizer;
    std::deque<std::string> actual_split, expected_split, src;
    char delimiter;
    bool keep_delimiter = true;

    delimiter = '^';
    src = {"A^X", "+", "B^1-C"};
    expected_split = {"A", "^", "X", "+", "B", "^", "1-C"};
    actual_split = tokenizer.split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '-';
    src = {"A^X", "-", "B^1-C"};
    expected_split = {"A^X", "-", "B^1", "-", "C"};
    actual_split = tokenizer.split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = ' ';
    src = {"  X^0 + X^1 +X^2    = 0   "};
    expected_split = {"X^0", "+", "X^1", "+X^2", "=",  "0"};
    actual_split = tokenizer.split_by_delimiter(src, delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '+';
    src = {"X^0", "+", "X^1", "+X^2", "=",  "0"};
    expected_split = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    actual_split = tokenizer.split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '-';
    src = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    expected_split = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    actual_split = tokenizer.split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);


    delimiter = '^';
    src = {"X^0", "+", "X^1", "+", "X^2", "=",  "0"};
    expected_split = {"X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "2", "=",  "0"};
    actual_split = tokenizer.split_by_delimiter(src, delimiter, keep_delimiter);
    EXPECT_EQ(actual_split, expected_split);
}


TEST(TestParser, SplitEquation) {
    Tokenizer tokenizer;
    std::string equation;
    std::deque<std::string> actual_split, expected_split;

    equation = "  X^0 + X^1 +X^2    = 0   ";
    expected_split = {"X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "2", "=",  "0"};
    actual_split = tokenizer.split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);


    equation = "  X ^ 0+X^  1 +X^+2    = 0  =0=++X^123  ";
    expected_split = { "X", "^", "0", "+", "X", "^", "1", "+", "X", "^", "+", "2", "=", "0", "=", "0", "=", "+", "+", "X", "^", "123" };
    actual_split = tokenizer.split_equation(equation);
    EXPECT_EQ(actual_split, expected_split);

}
