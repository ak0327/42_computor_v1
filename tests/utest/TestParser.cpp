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

    str = " abc    ";
    //     01234
    //   st^^end
    start = 0;
    expected_end = 1;
    Parser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = " abc    ";
    //     01234
    //    st^end
    start = 1;
    expected_end = start;
    Parser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
}


TEST(TestParser, TestParseTermOK) {
    Parser parser;
    std::deque<s_token> tokens;
    std::deque<s_token>::const_iterator itr;
    s_term actual_term, expected_term;

    tokens = {
            {.word="1", .kind=TermCoef},
    };
    expected_term = {
            .coefficient = 1,
            .variable = '\0',
            .degree = 0,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="X", .kind=TermBase},
    };
    expected_term = {
            .coefficient = 1,
            .variable = 'X',
            .degree = 1,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="2", .kind=TermCoef},
            {.word="*", .kind=OperatorMul},
            {.word="X", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=TermPower},
    };
    expected_term = {
            .coefficient = 2,
            .variable = 'X',
            .degree = 0,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="-", .kind=OperatorMinus},
            {.word="5.2", .kind=TermCoef},
            {.word="a", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="2", .kind=TermPower},
            {.word="=", .kind=OperatorEqual},
            {.word="2", .kind=TermCoef},
    };
    expected_term = {
            .coefficient = -5.2,
            .variable = 'a',
            .degree = 2,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=TermPower},
    };
    expected_term = {
            .coefficient = 1,
            .variable = 'x',
            .degree = 0,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=TermPower},
    };
    expected_term = {
            .coefficient = 1,
            .variable = 'x',
            .degree = 0,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="-", .kind=OperatorPlus},
            {.word="0.02500", .kind=TermCoef},
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="05", .kind=TermPower},
    };
    expected_term = {
            .coefficient = 0.02500,
            .variable = 'x',
            .degree = 5,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="2147483647", .kind=TermCoef},
            {.word="a", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="2147483647", .kind=TermPower},
    };
    expected_term = {
            .coefficient = 2147483647,
            .variable = 'a',
            .degree = 2147483647,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="x", .kind=TermBase},  // ここまで解析
            {.word="*", .kind=OperatorMul},
            {.word="y", .kind=TermBase},
    };
    expected_term = {
            .coefficient = 1,
            .variable = 'x',
            .degree = 1,
    };
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


}


TEST(TestParser, TestParseTermNG) {
    Parser parser;
    std::deque<s_token> tokens;
    std::deque<s_token>::const_iterator itr;
    s_term actual_term, expected_term;

    tokens = {
            {.word="1", .kind=TermCoef},
            {.word="*", .kind=OperatorMul},
            {.word="1", .kind=TermCoef},
    };
    expected_term = {};
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="^", .kind=TermPowSymbol},
    };
    expected_term = {};
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="x", .kind=TermBase},
    };
    expected_term = {};
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="1.2", .kind=TermPower},
    };
    expected_term = {};
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="-", .kind=OperatorMinus},
            {.word="2", .kind=TermPower},
    };
    expected_term = {};
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
    };
    expected_term = {};
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="=", .kind=OperatorEqual},
    };
    expected_term = {};
    itr = tokens.begin();
    actual_term = Parser::parse_term(tokens, &itr);
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);
}


TEST(TestParser, TestParseEquationByTokenNG) {
    Parser parser;
    std::deque<s_token> tokens;
    Status expected_status, actual_status;

    tokens = {};
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="y", .kind=TermBase},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="1", .kind=TermCoef},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="=", .kind=OperatorEqual},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="+", .kind=OperatorPlus},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="x", .kind=TermBase},
            {.word="*", .kind=OperatorMul},
            {.word="y", .kind=TermBase},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="*", .kind=OperatorMul},
            {.word="y", .kind=TermBase},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="-", .kind=OperatorMinus},
            {.word="y", .kind=TermBase},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="y", .kind=TermBase},
            {.word="=", .kind=OperatorEqual},
            {.word="=", .kind=OperatorEqual},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="y", .kind=TermBase},
            {.word="=", .kind=OperatorEqual},
            {.word="1", .kind=TermCoef},
            {.word="=", .kind=OperatorEqual},
            {.word="1", .kind=TermCoef},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);


    tokens = {
            {.word="x", .kind=TermBase},
            {.word="^", .kind=TermPowSymbol},
            {.word="3", .kind=TermPower},
            {.word="=", .kind=OperatorEqual},
            {.word="1", .kind=TermCoef},
    };
    actual_status = parser.parse_equation(tokens);
    expected_status = Status::FAILURE;
    EXPECT_EQ(expected_status, actual_status);

}

TEST(TestParser, TestParseEquationByStringOK) {
    Tokenizer t;
    Parser *p;
    std::string equation;
    std::deque<s_token> tokens;
    std::map<int, double> actual_poly, expected_poly;
    Status tokenizer_res, parser_res;

    // simple equation
    equation = "X^0 + X^1 + X^2 = 0";
    expected_poly = {
            {0, 1},
            {1, 1},
            {2, 1},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);


    equation = "  -X^0 + 2X^1 - 100X^2    = 100   ";
    expected_poly = {
            {0, 101},
            {1, -2},
            {2, 100},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);


    equation = "A^0 + A^1 + A^2 = 0";
    expected_poly = {
            {0, 1},
            {1, 1},
            {2, 1},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);

    equation = "0 = X^0 + X^1 + X^2";
    expected_poly = {
            {0, 1},
            {1, 1},
            {2, 1},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);

    // subject
    equation = "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0";
    expected_poly = {
            {0, -4},
            {1, -4},
            {2, 9.3},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);

    equation = "5 * X^0 + 4 * X^1 = 4 * X^0";
    expected_poly = {
            {0, 1},
            {1, 4},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);


    equation = "0 = 0";
    expected_poly = {
            {0, 0},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);


    equation = "1 = 0";
    expected_poly = {
            {0, 1},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);


    equation = "0 = 1";
    expected_poly = {
            {0, 1},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::SUCCESS, parser_res);
    EXPECT_EQ(expected_poly, actual_poly);

}


TEST(TestParser, TestParseEquationByStringNG) {
    Tokenizer t;
    Parser *p;
    std::string equation;
    std::deque<s_token> tokens;
    Status tokenizer_res, parser_res;

    // subject
    // equation = "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0";
    // tokenizer_res = t.tokenize(equation);
    // tokens = t.tokens();
    // p = new Parser();
    // parser_res = p->parse_equation(tokens);
    // delete p;
    // EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    // EXPECT_EQ(Status::FAILURE, parser_res);
    //
    // // simple test
    // equation = "X^3 = 0";
    // tokenizer_res = t.tokenize(equation);
    // tokens = t.tokens();
    // p = new Parser();
    // parser_res = p->parse_equation(tokens);
    // delete p;
    // EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    // EXPECT_EQ(Status::FAILURE, parser_res);
    //
    //
    // equation = "X^0 == 0";
    // tokenizer_res = t.tokenize(equation);
    // tokens = t.tokens();
    // p = new Parser();
    // parser_res = p->parse_equation(tokens);
    // delete p;
    // EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    // EXPECT_EQ(Status::FAILURE, parser_res);
    //
    //
    // equation = "X^0 = 0 = 0";
    // tokenizer_res = t.tokenize(equation);
    // tokens = t.tokens();
    // p = new Parser();
    // parser_res = p->parse_equation(tokens);
    // delete p;
    // EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    // EXPECT_EQ(Status::FAILURE, parser_res);


    equation = "= X^0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::FAILURE, parser_res);


    equation = "^ X^0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::FAILURE, parser_res);


    equation = "X^0 =";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    EXPECT_EQ(Status::FAILURE, parser_res);


    // equation = "X^0 + Y^0";
    // tokenizer_res = t.tokenize(equation);
    // tokens = t.tokens();
    // p = new Parser();
    // parser_res = p->parse_equation(tokens);
    // delete p;
    // EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    // EXPECT_EQ(Status::FAILURE, parser_res);
    //
    //
    // equation = "X^1 + Y^2 = 0";
    // tokenizer_res = t.tokenize(equation);
    // tokens = t.tokens();
    // p = new Parser();
    // parser_res = p->parse_equation(tokens);
    // delete p;
    // EXPECT_EQ(Status::SUCCESS, tokenizer_res);
    // EXPECT_EQ(Status::FAILURE, parser_res);
}
