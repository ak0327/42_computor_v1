#include "computor.hpp"
#include "gtest/gtest.h"

class TestComputor : public ::testing::Test {
 protected:
    std::streambuf* old_cout;
    std::streambuf* old_cerr;
    std::stringstream captured_cout;
    std::stringstream captured_cerr;

    virtual void SetUp() {
        old_cout = std::cout.rdbuf(captured_cout.rdbuf());
        old_cerr = std::cerr.rdbuf(captured_cerr.rdbuf());
    }

    virtual void TearDown() {
        std::cout.rdbuf(old_cout);
        std::cerr.rdbuf(old_cerr);
    }
};

////////////////////////////////////////////////////////////////////////////////

TEST_F(TestComputor, SubjectEquation01) {
    std::string equation = "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 9.3 * X^2 - 4 * X - 4 = 0\n"
                                  "Polynomial degree: 2\n"
                                  "Discriminant is positive, the two solutions are:\n"
                                  " 0.905239\n"
                                  "-0.475131\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, SubjectEquation02) {
    std::string equation = "5 * X^0 + 4 * X^1 = 4 * X^0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 4 * X + 1 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  "-0.25\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, SubjectEquation03) {
    std::string equation = "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 5.6 * X^3 + 6 * X - 5 = 0\n"
                                  "Polynomial degree: 3\n"
                                  "The polynomial degree is strictly greater than 2, I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

////////////////////////////////////////////////////////////////////////////////

TEST_F(TestComputor, ConstantEquationIndeterminate01) {
    std::string equation = "1 = 1";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationIndeterminate02) {
    std::string equation = " +1 = +1 ";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationIndeterminate03) {
    std::string equation = "-1=-1";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationIndeterminate04) {
    std::string equation = "123.567 = 123.567";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}


TEST_F(TestComputor, ConstantEquationIndeterminate05) {
    std::string equation = "X=X";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationIndeterminate06) {
    std::string equation = "+0*X^0= 0*X +1X^0 - 1X^0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationIndeterminate07) {
    std::string equation = "+X^100 = X^100";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationIndeterminate08) {
    std::string equation = "+0 = 0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationIndeterminate09) {
    std::string equation = "0*X^2 + 0*X + 0 = 0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 0 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "The equation is indeterminate, infinite solutions.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

// -----------------------------------------------------------------------------

TEST_F(TestComputor, ConstantEquationNoSolution01) {
    std::string equation = " 1 =  0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 1 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationNoSolution02) {
    std::string equation = " -1 =+1 ";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 2 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationNoSolution03) {
    std::string equation = " -1 = +1.0  *  X ^0 ";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 2 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationNoSolution04) {
    std::string equation = " 1X^0 = -1.0 ";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 2 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ConstantEquationNoSolution05) {
    std::string equation = "2 = 1";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 1 = 0\n"
                                  "Polynomial degree: 0\n"
                                  "I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

////////////////////////////////////////////////////////////////////////////////

TEST_F(TestComputor, LinearEquation01) {
    std::string equation = "X = 0";
    std::string expected_stdout = "Reduced form     : 1 * X = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  "0\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation02) {
    std::string equation = "X = 1";
    std::string expected_stdout = "Reduced form     : 1 * X - 1 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  " 1\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation03) {
    std::string equation = "X = -  5";
    std::string expected_stdout = "Reduced form     : 1 * X + 5 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  "-5\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation04) {
    std::string equation = "2 * X = 4";
    std::string expected_stdout = "Reduced form     : 2 * X - 4 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  " 2\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation05) {
    std::string equation = "0.5 * X = 1";
    std::string expected_stdout = "Reduced form     : 0.5 * X - 1 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  " 2\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation06) {
    std::string equation = "-3 * X = 9";
    std::string expected_stdout = "Reduced form     : 3 * X + 9 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  "-3\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation07) {
    std::string equation = "100.0X + 3.5 = 5";
    std::string expected_stdout = "Reduced form     : 100 * X - 1.5 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  " 0.015\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation08) {
    std::string equation = "1000.0X + 3.0 = 5";
    std::string expected_stdout = "Reduced form     : 1000 * X - 2 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  " 0.002\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation09) {
    std::string equation = " - 100a + 101 = 1.0";
    std::string expected_stdout = "Reduced form     : 100 * a - 100 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  " 1\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, LinearEquation10) {
    std::string equation = " - 100  a + 101 = 1.0";
    std::string expected_stdout = "Reduced form     : 100 * a - 100 = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  " 1\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}


////////////////////////////////////////////////////////////////////////////////

TEST_F(TestComputor, ValidEquation02) {
    std::string equation = "2X^2 + 3X - 5 = 0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 2 * X^2 + 3 * X - 5 = 0\n"
                                  "Polynomial degree: 2\n"
                                  "Discriminant is positive, the two solutions are:\n"
                                  " 1\n"
                                  "-2.5\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

////////////////////////////////////////////////////////////////////////////////

TEST_F(TestComputor, InvalidDegree01) {
    std::string equation = "X^3 = 0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 1 * X^3 = 0\n"
                                  "Polynomial degree: 3\n"
                                  "The polynomial degree is strictly greater than 2, I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidDegree02) {
    std::string equation = "x^3 - 2x + 1 = 0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 1 * x^3 - 2 * x + 1 = 0\n"
                                  "Polynomial degree: 3\n"
                                  "The polynomial degree is strictly greater than 2, I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidDegree03) {
    std::string equation = "X^2147483647 = 0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 1 * X^2147483647 = 0\n"
                                  "Polynomial degree: 2147483647\n"
                                  "The polynomial degree is strictly greater than 2, I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidDegree04) {
    std::string equation = "0 * X^2147483647 + 1 * X^3 = 0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 1 * X^3 = 0\n"
                                  "Polynomial degree: 3\n"
                                  "The polynomial degree is strictly greater than 2, I can't solve.\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}


////////////////////////////////////////////////////////////////////////////////

TEST_F(TestComputor, InvalidEquationEmpty01) {
    std::string equation = "";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] empty equation\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquationEmpty02) {
    std::string equation = "  ";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] empty equation\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquation01) {
    std::string equation = "+";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] syntax error: unexpected token: +\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquation04) {
    std::string equation = "-";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] syntax error: unexpected token: -\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquation05) {
    std::string equation = "*";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] syntax error: unexpected token: *\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquation06) {
    std::string equation = "^";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] syntax error: unexpected token: ^\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquation07) {
    std::string equation = "=";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] syntax error: unexpected token: =\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquation08) {
    std::string equation = ".";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] syntax error: unexpected token: .\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquation09) {
    std::string equation = "1 == 2";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] syntax error: unexpected token: =\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidEquation10) {
    std::string equation = "This is not an equation";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] syntax error: unexpected token: This\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}
