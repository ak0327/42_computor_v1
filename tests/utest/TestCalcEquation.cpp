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

TEST_F(TestComputor, ValidEquation01) {
    std::string equation = "X = 0";
    std::string expected_stdout = "Reduced form     : 1 * X = 0\n"
                                  "Polynomial degree: 1\n"
                                  "The solution is:\n"
                                  "0.00\n";
    std::string expected_stderr = "";
    int result = Computor::calc_equation(equation);

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, ValidEquation02) {
    std::string equation = "2X^2 + 3X - 5 = 0";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "Reduced form     : 2 * X^2 + 3 * X - 5 = 0\n"
                                  "Polynomial degree: 2\n"
                                  "Discriminant is positive, the two solutions are:\n"
                                  " 1.00\n"
                                  "-2.50\n";
    std::string expected_stderr = "";

    EXPECT_EQ(EXIT_SUCCESS, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}

TEST_F(TestComputor, InvalidDegree01) {
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

////////////////////////////////////////////////////////////////////////////////

TEST_F(TestComputor, ConstantEquation01) {
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

TEST_F(TestComputor, ConstantEquation02) {
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

TEST_F(TestComputor, ConstantEquation03) {
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

TEST_F(TestComputor, ConstantEquation04) {
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


TEST_F(TestComputor, ConstantEquation05) {
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

TEST_F(TestComputor, ConstantEquation06) {
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

TEST_F(TestComputor, ConstantEquation07) {
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


////////////////////////////////////////////////////////////////////////////////

TEST_F(TestComputor, InvalidEquation01) {
    std::string equation = "This is not an equation";
    int result = Computor::calc_equation(equation);
    std::string expected_stdout = "";
    std::string expected_stderr = "[Error] unexpected token [This]\n";

    EXPECT_EQ(EXIT_FAILURE, result);
    EXPECT_EQ(expected_stdout, captured_cout.str());
    EXPECT_EQ(expected_stderr, captured_cerr.str());
}
