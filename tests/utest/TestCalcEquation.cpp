#include "computor.hpp"
#include "gtest/gtest.h"


struct TestCase {
    std::string equation;
    int expected_result;
    std::string expected_stdout;
    std::string expected_stderr;
    std::size_t line;
};


std::ostream &operator<<(std::ostream &os, const TestCase& tc) {
    os << "Equation: " << tc.equation << ", "
       << "Expected Result: " << tc.expected_result << ", "
       << "Expected Stdout: " << tc.expected_stdout << ", "
       << "Expected Stderr: " << tc.expected_stderr;
    return os;
}


class TestComputor : public ::testing::TestWithParam<TestCase> {
 protected:
    std::streambuf *old_cout;
    std::streambuf *old_cerr;
    std::stringstream captured_cout;
    std::stringstream captured_cerr;

    virtual void SetUp() override {
        old_cout = std::cout.rdbuf(captured_cout.rdbuf());
        old_cerr = std::cerr.rdbuf(captured_cerr.rdbuf());
    }

    virtual void TearDown() override {
        std::cout.rdbuf(old_cout);
        std::cerr.rdbuf(old_cerr);
    }
};


TEST_P(TestComputor, TestComputorOutput) {
    TestCase param = GetParam();

    std::string &equation = param.equation;
    int expected_result = param.expected_result;
    std::string &expected_stdout = param.expected_stdout;
    std::string &expected_stderr = param.expected_stderr;

    int actual_result = Computor::calc_equation(equation);

    EXPECT_EQ(expected_result, actual_result) << " at L" << param.line;
    EXPECT_EQ(expected_stdout, captured_cout.str()) << " at L" << param.line;
    EXPECT_EQ(expected_stderr, captured_cerr.str()) << " at L" << param.line;
}

////////////////////////////////////////////////////////////////////////////////

INSTANTIATE_TEST_SUITE_P(
        SubjectEquation,
        TestComputor,
        ::testing::Values(
                TestCase{
                        .equation        = "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 9.3 * X^2 - 4 * X - 4 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is positive, the two solutions are:\n"
                                           " 0.905239\n"
                                           "-0.475131\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "5 * X^0 + 4 * X^1 = 4 * X^0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 4 * X + 1 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           "-0.25\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 5.6 * X^3 + 6 * X - 5 = 0\n"
                                           "Polynomial degree: 3\n"
                                           "The polynomial degree is strictly greater than 2, I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                }
        )
);


////////////////////////////////////////////////////////////////////////////////


INSTANTIATE_TEST_SUITE_P(
        ConstantEquationIndeterminate,
        TestComputor,
        ::testing::Values(
                TestCase{
                        .equation        = "1 = 1",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = " +1 = +1 ",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "-1=-1",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "123.567 = 123.567",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "X=X",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "+0*X^0= 0*X +1X^0 - 1X^0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "+X^100 = X^100",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "+0 = 0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__

                },
                TestCase{
                        .equation        = "0*X^2 + 0*X + 0 = 0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 0 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "The equation is indeterminate, infinite solutions.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                }
        )
);


INSTANTIATE_TEST_SUITE_P(
        ConstantEquationNoSolution,
        TestComputor,
        ::testing::Values(
                TestCase{
                        .equation        = " 1 =  0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 1 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = " -1 =+1 ",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 2 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = " -1 = +1.0  *  X ^0 ",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 2 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = " 1X^0 = -1.0 ",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 2 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "2 = 1",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 1 = 0\n"
                                           "Polynomial degree: 0\n"
                                           "I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                }
        )
);


////////////////////////////////////////////////////////////////////////////////


INSTANTIATE_TEST_SUITE_P(
        LinearEquation,
        TestComputor,
        ::testing::Values(
                TestCase{
                        .equation        = "X = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1 * X = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           "0\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "X = 1",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1 * X - 1 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           " 1\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "X = -  5",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1 * X + 5 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           "-5\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "2 * X = 4",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 2 * X - 4 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           " 2\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "0.5 * X = 1",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 0.5 * X - 1 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           " 2\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "-3 * X = 9",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 3 * X + 9 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           "-3\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "100.0X + 3.5 = 5",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 100 * X - 1.5 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           " 0.015\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "1000.0X + 3.0 = 5",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1000 * X - 2 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           " 0.002\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = " - 100a + 101 = 1.0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 100 * a - 100 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           " 1\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = " - 100  a + 101 = 1.0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 100 * a - 100 = 0\n"
                                           "Polynomial degree: 1\n"
                                           "The solution is:\n"
                                           " 1\n",
                        .expected_stderr = "",
                        .line = __LINE__
                }
        )
);


////////////////////////////////////////////////////////////////////////////////


INSTANTIATE_TEST_SUITE_P(
        QuadracitEquation2RealSolutions,
        TestComputor,
        ::testing::Values(
                TestCase{
                        .equation        = "2X^2 + 3X - 5 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 2 * X^2 + 3 * X - 5 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is positive, the two solutions are:\n"
                                           " 1\n"
                                           "-2.5\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "0 =2*X^2 + 3X - 5",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 2 * X^2 + 3 * X - 5 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is positive, the two solutions are:\n"
                                           " 1\n"
                                           "-2.5\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = " +2 * X ^ 2 + 3X =+5.00000000000  ",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 2 * X^2 + 3 * X - 5 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is positive, the two solutions are:\n"
                                           " 1\n"
                                           "-2.5\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "0.000001 * X^2 + 0.000001 * X - 0.000001 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1e-06 * X^2 + 1e-06 * X - 1e-06 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is positive, the two solutions are:\n"
                                           " 0.618034\n"
                                           "-1.61803\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "0.5 * X^2 + 1.5 * X + 1.0 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 0.5 * X^2 + 1.5 * X + 1 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is positive, the two solutions are:\n"
                                           "-1\n"
                                           "-2\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "1000000 * X^2 + 500000 * X - 250000 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1e+06 * X^2 + 500000 * X - 250000 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is positive, the two solutions are:\n"
                                           " 0.309017\n"
                                           "-0.809017\n",
                        .expected_stderr = "",
                        .line = __LINE__
                }
        )
);


INSTANTIATE_TEST_SUITE_P(
        QuadracitEquation2ComplexSolutions,
        TestComputor,
        ::testing::Values(
                TestCase{
                        .equation        = "X^2 + 2X + 5 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1 * X^2 + 2 * X + 5 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is negative, the two solutions are:\n"
                                           "-1+2i\n"
                                           "-1-2i\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "3X^2 + 2X + 7 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 3 * X^2 + 2 * X + 7 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is negative, the two solutions are:\n"
                                           "-0.333333+1.49071i\n"
                                           "-0.333333-1.49071i\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "0.5X^2 + 1.5X + 2 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 0.5 * X^2 + 1.5 * X + 2 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is negative, the two solutions are:\n"
                                           "-1.5+1.32288i\n"
                                           "-1.5-1.32288i\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "2X^2 + 4X + 8 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 2 * X^2 + 4 * X + 8 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is negative, the two solutions are:\n"
                                           "-1+1.73205i\n"
                                           "-1-1.73205i\n",
                        .expected_stderr = "",
                        .line = __LINE__
                }
        )
);


INSTANTIATE_TEST_SUITE_P(
        QuadracitEquation1RealSolution,
        TestComputor,
        ::testing::Values(
                TestCase{
                        .equation        = "X^2 - 6X + 9 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1 * X^2 - 6 * X + 9 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is zero, the solution is:\n"
                                           " 3\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "4X^2 - 4X + 1 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 4 * X^2 - 4 * X + 1 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is zero, the solution is:\n"
                                           " 0.5\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "0.25X^2 - X + 1 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 0.25 * X^2 - 1 * X + 1 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is zero, the solution is:\n"
                                           " 2\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "X^2 - 4X + 4 = 0",
                        .expected_result = EXIT_SUCCESS,
                        .expected_stdout = "Reduced form     : 1 * X^2 - 4 * X + 4 = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Discriminant is zero, the solution is:\n"
                                           " 2\n",
                        .expected_stderr = "",
                        .line = __LINE__
                }
        )
);


////////////////////////////////////////////////////////////////////////////////

INSTANTIATE_TEST_SUITE_P(
        ErrorCasesDegreeTooLarge,
        TestComputor,
        ::testing::Values(
                TestCase{
                        .equation        = "X^3 = 0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 1 * X^3 = 0\n"
                                           "Polynomial degree: 3\n"
                                           "The polynomial degree is strictly greater than 2, I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "x^3 - 2x + 1 = 0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 1 * x^3 - 2 * x + 1 = 0\n"
                                           "Polynomial degree: 3\n"
                                           "The polynomial degree is strictly greater than 2, I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "X^2147483647 = 0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 1 * X^2147483647 = 0\n"
                                           "Polynomial degree: 2147483647\n"
                                           "The polynomial degree is strictly greater than 2, I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                },
                TestCase{
                        .equation        = "0 * X^2147483647 + 1 * X^3 = 0",
                        .expected_result = EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 1 * X^3 = 0\n"
                                           "Polynomial degree: 3\n"
                                           "The polynomial degree is strictly greater than 2, I can't solve.\n",
                        .expected_stderr = "",
                        .line = __LINE__
                }
        )
);


INSTANTIATE_TEST_SUITE_P(
        InvalidEquation,
        TestComputor,
        ::testing::Values(
                TestCase{"",    EXIT_FAILURE, "", "[Error] empty equation\n", __LINE__},
                TestCase{"  ",  EXIT_FAILURE, "", "[Error] empty equation\n", __LINE__},
                TestCase{"+",   EXIT_FAILURE, "", "[Error] syntax error: unexpected token: +\n", __LINE__},
                TestCase{"-",   EXIT_FAILURE, "", "[Error] syntax error: unexpected token: -\n", __LINE__},
                TestCase{"*",   EXIT_FAILURE, "", "[Error] syntax error: unexpected token: *\n", __LINE__},
                TestCase{"^",   EXIT_FAILURE, "", "[Error] syntax error: unexpected token: ^\n", __LINE__},
                TestCase{"=",   EXIT_FAILURE, "", "[Error] syntax error: unexpected token: =\n", __LINE__},
                TestCase{".",   EXIT_FAILURE, "", "[Error] syntax error: unexpected token: .\n", __LINE__},
                TestCase{"This is not an equation", EXIT_FAILURE, "", "[Error] syntax error: unexpected token: This\n", __LINE__},
                TestCase{"1 == 2",      EXIT_FAILURE, "", "[Error] syntax error: unexpected token: =\n", __LINE__},
                TestCase{"X^-1 = 0",    EXIT_FAILURE, "", "[Error] syntax error: unexpected token: -\n", __LINE__},
                TestCase{"X^+1 = 0",    EXIT_FAILURE, "", "[Error] syntax error: unexpected token: +\n", __LINE__},
                TestCase{"X^1.0 = 0",   EXIT_FAILURE, "", "[Error] syntax error: unexpected token: 1.0\n", __LINE__},
                TestCase{"X^a = 0",     EXIT_FAILURE, "", "[Error] syntax error: unexpected token: a\n", __LINE__},
                TestCase{"X^1a = 0",    EXIT_FAILURE, "", "[Error] syntax error: unexpected token: a\n", __LINE__},
                TestCase{"X^1^1 = 0",   EXIT_FAILURE, "", "[Error] syntax error: unexpected token: ^\n", __LINE__},
                TestCase{"X^^1 = 0",    EXIT_FAILURE, "", "[Error] syntax error: unexpected token: ^\n", __LINE__}
        )
);

std::string DMAX = "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368";
INSTANTIATE_TEST_SUITE_P(
        CalculationError,
        TestComputor,
        ::testing::Values(
                TestCase{DMAX+"x + " + DMAX + "x = 1",    EXIT_FAILURE, "", "[Error] calculation error: coefficient is infinity at degree 1\n", __LINE__},
                TestCase{"-" + DMAX + "x - " + DMAX + "x = 1",    EXIT_FAILURE, "", "[Error] calculation error: coefficient is infinity at degree 1\n", __LINE__},
                TestCase{"x^" + DMAX + " + x = 1",    EXIT_FAILURE, "", "[Error] syntax error: unexpected token: " + DMAX + "\n", __LINE__},
                TestCase{DMAX + "x + " + DMAX + "x = 1",    EXIT_FAILURE, "", "[Error] calculation error: coefficient is infinity at degree 1\n", __LINE__},
                TestCase{DMAX + "x^2 + " + DMAX + "x + " + DMAX + "= " + DMAX,    EXIT_FAILURE,
                        .expected_stdout = "Reduced form     : 1.79769e+308 * x^2 + 1.79769e+308 * x = 0\n"
                                           "Polynomial degree: 2\n"
                                           "Calculation error occurred, I can't solve.\n", "", __LINE__}
        )
);
