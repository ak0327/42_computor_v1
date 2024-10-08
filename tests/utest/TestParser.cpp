#include "TestParser.hpp"
#include "Result.hpp"
#include "gtest/gtest.h"

TEST(TestParser, TestSkipSP) {
    std::string str;
    std::size_t start, actual_end, expected_end;

    str = " 1";
    //     01234
    //   st^^end
    start = 0;
    expected_end = 1;
    TestParser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "   1";
    //     01234
    //   st^  ^end
    start = 0;
    expected_end = 3;
    TestParser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "   1";
    //     01234
    //   st^  ^end
    start = 0;
    expected_end = 3;
    TestParser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "";
    //     01234
    //   st^end
    start = 0;
    expected_end = start;
    TestParser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "";
    //     01234
    //       st^end
    start = 4;
    expected_end = start;
    TestParser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = "abc";
    //     01234
    //   st^end
    start = 0;
    expected_end = start;
    TestParser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);

    str = " abc    ";
    //     01234
    //   st^^end
    start = 0;
    expected_end = 1;
    TestParser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);


    str = " abc    ";
    //     01234
    //    st^end
    start = 1;
    expected_end = start;
    TestParser::skip_sp(str, start, &actual_end);
    EXPECT_EQ(expected_end, actual_end);
}


TEST(TestParser, TestParseTermOK) {
    Parser parser;
    std::deque<s_token> tokens;
    std::deque<s_token>::const_iterator itr, end;
    Result<s_term, Computor::Status> result;
    s_term actual_term, expected_term;

    tokens = {
            {.word="1", .kind=Integer},
    };
    expected_term = {
            .coefficient = 1,
            .variable = '\0',
            .degree = 0,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="X", .kind=Char},
    };
    expected_term = {
            .coefficient = 1,
            .variable = 'X',
            .degree = 1,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="2", .kind=Integer},
            {.word="*", .kind=OperatorMul},
            {.word="X", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=Integer},
    };
    expected_term = {
            .coefficient = 2,
            .variable = 'X',
            .degree = 0,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="-", .kind=OperatorMinus},
            {.word="5.2", .kind=Decimal},
            {.word="a", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="2", .kind=Integer},
            {.word="=", .kind=OperatorEqual},
            {.word="2", .kind=Integer},
    };
    expected_term = {
            .coefficient = -5.2,
            .variable = 'a',
            .degree = 2,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=Integer},
    };
    expected_term = {
            .coefficient = 1,
            .variable = 'x',
            .degree = 0,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="0", .kind=Integer},
    };
    expected_term = {
            .coefficient = 1,
            .variable = 'x',
            .degree = 0,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="-", .kind=OperatorPlus},
            {.word="0.02500", .kind=Decimal},
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="05", .kind=Integer},
    };
    expected_term = {
            .coefficient = 0.02500,
            .variable = 'x',
            .degree = 5,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="2147483647", .kind=Integer},
            {.word="a", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="2147483647", .kind=Integer},
    };
    expected_term = {
            .coefficient = 2147483647,
            .variable = 'a',
            .degree = 2147483647,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


    tokens = {
            {.word="x", .kind=Char},  // ここまで解析
            {.word="*", .kind=OperatorMul},
            {.word="y", .kind=Char},
    };
    expected_term = {
            .coefficient = 1,
            .variable = 'x',
            .degree = 1,
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_ok());
    actual_term = result.ok_value();
    EXPECT_EQ(expected_term.coefficient, actual_term.coefficient);
    EXPECT_EQ(expected_term.variable, actual_term.variable);
    EXPECT_EQ(expected_term.degree, actual_term.degree);


}


TEST(TestParser, TestParseTermNG) {
    Parser parser;
    std::deque<s_token> tokens;
    std::deque<s_token>::const_iterator itr, end;
    Result<s_term, Computor::Status> result;

    tokens = {
            {.word="1", .kind=Integer},
            {.word="*", .kind=OperatorMul},
            {.word="1", .kind=Integer},
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_err());


    tokens = {
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="^", .kind=TermPowSymbol},
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_err());


    tokens = {
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="x", .kind=Char},
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_err());


    tokens = {
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="1.2", .kind=Decimal},
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_err());


    tokens = {
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="-", .kind=OperatorMinus},
            {.word="2", .kind=Integer},
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_err());


    tokens = {
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_err());


    tokens = {
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="=", .kind=OperatorEqual},
    };
    itr = tokens.begin();
    end = tokens.end();
    result = TestParser::parse_term(&itr, end);
    EXPECT_TRUE(result.is_err());
}


TEST(TestParser, TestParseEquationByTokenNG) {
    Parser parser;
    std::deque<s_token> tokens;
    Result<Polynomials, ErrMsg> parse_res;

    tokens = {};
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="y", .kind=Char},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="1", .kind=Integer},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="=", .kind=OperatorEqual},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="+", .kind=OperatorPlus},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="x", .kind=Char},
            {.word="*", .kind=OperatorMul},
            {.word="y", .kind=Char},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="*", .kind=OperatorMul},
            {.word="y", .kind=Char},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="+", .kind=OperatorPlus},
            {.word="-", .kind=OperatorMinus},
            {.word="y", .kind=Char},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="y", .kind=Char},
            {.word="=", .kind=OperatorEqual},
            {.word="=", .kind=OperatorEqual},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="y", .kind=Char},
            {.word="=", .kind=OperatorEqual},
            {.word="1", .kind=Decimal},
            {.word="=", .kind=OperatorEqual},
            {.word="1", .kind=Decimal},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());


    tokens = {
            {.word="x", .kind=Char},
            {.word="^", .kind=TermPowSymbol},
            {.word="3", .kind=Integer},
            {.word="=", .kind=OperatorEqual},
            {.word="1", .kind=Decimal},
    };
    parse_res = parser.parse_equation(tokens);
    EXPECT_TRUE(parse_res.is_err());

}

TEST(TestParser, TestParseEquationByStringOK) {
    Tokenizer t;
    Parser *p;
    std::string equation;
    std::string actual_form, expected_form;
    std::deque<s_token> tokens;
    std::map<int, double> actual_poly, expected_poly;
    Result<Tokens, ErrMsg> tokenizer_res;
    Result<Polynomials, ErrMsg> parser_res;

    // simple equation
    equation = "X^0 + X^1 + X^2 = 0";
    expected_form = "1 * X^2 + 1 * X + 1 = 0";
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
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);


    equation = "  -X^0 + 2X^1 - 100X^2    = 100   ";
    expected_form = "100 * X^2 - 2 * X + 101 = 0";
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
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);


    equation = "A^0 + A^1 + A^2 = 0";
    expected_form = "1 * A^2 + 1 * A + 1 = 0";
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
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);

    equation = "0 = X^0 + X^1 + X^2";
    expected_form = "1 * X^2 + 1 * X + 1 = 0";
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
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);

    // subject
    equation = "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0";
    expected_form = "9.3 * X^2 - 4 * X - 4 = 0";
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
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);

    equation = "5 * X^0 + 4 * X^1 = 4 * X^0";
    expected_form = "4 * X + 1 = 0";
    expected_poly = {
            {0, 1},
            {1, 4},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);


    equation = "0 = 0";
    expected_form = "0 = 0";
    expected_poly = {
            {0, 0},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);


    equation = "1 = 0";
    expected_form = "1 = 0";
    expected_poly = {
            {0, 1},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);


    equation = "0 = 1";
    expected_form = "1 = 0";
    expected_poly = {
            {0, 1},
    };
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    actual_poly = p->polynomial();
    actual_form = p->reduced_form();
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());
    EXPECT_EQ(expected_poly, actual_poly);
    EXPECT_EQ(expected_form, actual_form);
}


TEST(TestParser, TestParseEquationByStringNG) {
    Tokenizer t;
    Parser *p;
    std::string equation;
    std::deque<s_token> tokens;
    Result<Tokens, ErrMsg> tokenizer_res;
    Result<Polynomials, ErrMsg> parser_res;

    // subject
    equation = "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());  // parseの段階では成立していれば3次でもOK

    // simple test
    equation = "X^3 = 0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_ok());  // parseの段階では成立していれば3次でもOK


    equation = "X^0 == 0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_err());


    equation = "X^0 = 0 = 0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_err());


    equation = "= X^0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_err());


    equation = "^ X^0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_err());


    equation = "X^0 =";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_ok());
    EXPECT_TRUE(parser_res.is_err());


    equation = "X^0 + Y^0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_err());  // X, Y
    EXPECT_TRUE(parser_res.is_err());


    equation = "X^1 + Y^2 = 0";
    tokenizer_res = t.tokenize(equation);
    tokens = t.tokens();
    p = new Parser();
    parser_res = p->parse_equation(tokens);
    delete p;
    EXPECT_TRUE(tokenizer_res.is_err());  // X, Y
    EXPECT_TRUE(parser_res.is_err());
}


void expect_eq_reduced_form(
        const std::string &equation,
        const std::string &expected_form,
        std::size_t line) {
    Tokenizer t;
    Parser p;
    std::string actual_form;

    t.tokenize(equation);
    p.parse_equation(t.tokens());
    actual_form = p.reduced_form();
    EXPECT_EQ(expected_form, actual_form) << " at L:" << line << std::endl;
}

TEST(TestParser, TestReducedForm) {
    std::string equation, expected_form;

    // subject
    equation = "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0";
    expected_form = "9.3 * X^2 - 4 * X - 4 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "5 * X^0 + 4 * X^1 = 4 * X^0";
    expected_form = "4 * X + 1 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0";
    expected_form = "5.6 * X^3 + 6 * X - 5 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    // test
    equation = "X^0 + X^1 + X^2 = 0";
    expected_form = "1 * X^2 + 1 * X + 1 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "X^2 - 2X^2 + X^1 - 1 * X^1 + 0*X^0 = X^0 + 2";
    expected_form = "1 * X^2 + 3 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "X^1 = 0";
    expected_form = "1 * X = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "X = 0";
    expected_form = "1 * X = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "x = x";
    expected_form = "0 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "-X = -1 * X";
    expected_form = "0 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "- 1 + 1 = 0";
    expected_form = "0 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "X^3 = 0";
    expected_form = "1 * X^3 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = " 0 = X^3";
    expected_form = "1 * X^3 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = " 2 * X^2147483647 - X^3 = 0";
    expected_form = "2 * X^2147483647 - 1 * X^3 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = " 1.025 -3456.789 X^3 = X^2 + 1 * X^0 - 1";
    expected_form = "3456.79 * X^3 + 1 * X^2 - 1.025 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = " +X^1 + X = 0";
    expected_form = "2 * X = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    std::string term = "+X^1";
    equation = "";
    for (int i = 0; i < 100; ++i) {
        equation += term;
    }
    equation += "= 0";
    expected_form = "100 * X = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "";
    term = "+ X^1 + X^2";
    for (int i = 0; i < 10000; ++i) {
        equation += term;
    }
    equation += "= 0";
    expected_form = "10000 * X^2 + 10000 * X = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    // equation = "";
    // term = "+ X^1 + X^2 - 10";
    // for (int i = 0; i < 50000; ++i) {
    //     equation += term;
    // }
    // equation += "= 0";
    // expected_form = "50000 * X^2 + 50000 * X - 500000 = 0";
    // expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "0 * X^0 - 0 * X^1 + 0 * X^2 + 0 - 0 - 0.0 = 0.0";
    expected_form = "0 = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);

    equation = "X^2 + 0*X^0 + X = 0";
    expected_form = "1 * X^2 + 1 * X = 0";
    expect_eq_reduced_form(equation, expected_form, __LINE__);


    // equation = "";
    // expected_form = "";
    // expect_eq_reduced_form(equation, expected_form, __LINE__);
    //
    // equation = "";
    // expected_form = "";
    // expect_eq_reduced_form(equation, expected_form, __LINE__);
    //
    // equation = "";
    // expected_form = "";
    // expect_eq_reduced_form(equation, expected_form, __LINE__);
}


TEST(TestParser, TestErrorEquation) {
    std::string equation, expected_form;

    (void)expected_form;

    equation = "X^2 + 0*Y^0 + X = 0";

    equation = "X^2 + 0*Y^0 + X^0 = 0";

    equation = "X^2 + 0*Y^0 + X^1 = 0";

    equation = "X^2 + 1*Y^1 + X^1 = 0";

    equation = "X^2 + 0*Y^1 + 0*X^1 = 0";
}
