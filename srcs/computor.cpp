#include "computor.hpp"
#include <iostream>
#include <limits>
#include "Calculator.hpp"
#include "Tokenizer.hpp"
#include "Result.hpp"
#include "Parser.hpp"

namespace Computor {

int calc_equation(const std::string &equation) noexcept(true) {
    Tokenizer tokenizer;
    Result<Tokens, ErrMsg> tokenize_result = tokenizer.tokenize(equation);
    if (tokenize_result.is_err()) {
        std::cerr << "[Error] " << tokenize_result.err_value() << std::endl;
        return EXIT_FAILURE;
    }
    Tokens tokens = tokenize_result.ok_value();

    Parser parser;
    Result<Polynomials, ErrMsg> parse_result = parser.parse_equation(tokens);
    if (parse_result.is_err()) {
        std::cerr << "[Error] " << parse_result.err_value() << std::endl;
        return EXIT_FAILURE;
    }
    parser.display_reduced_form();
    parser.display_polynomial_degree();
    Polynomials polynomial = parse_result.ok_value();

    Calculator calculator(polynomial);
    return calculator.solve_quadratic_equation();
}

double normalize_zero(double value) noexcept(true) {
    return value == 0.0 ? 0.0 : value;
}

double abs(double num) noexcept(true) {
    return num < 0 ? -num : num;
}

double sqrt(double num) noexcept(false) {
    if (Computor::isnan(num) || Computor::isinf(num)) {
        throw std::domain_error("Cannot calculate square root of NaN or inf");
    }
    if (num < 0) {
        throw std::domain_error("Cannot calculate square root of negative number");
    }
    if (num == 0 || num == 1) { return num; }

    double epsilon = 1e-15;
    double guess = (num < 1.0) ? 1.0 : num / 2.0;

    int max_iterations = 1000;
    for (int i = 0; i < max_iterations; ++i) {
        double difference = guess * guess - num;
        if (Computor::abs(difference) < epsilon * num) {
            break;
        }
        guess = (guess + num / guess) / 2.0;
    }
    return guess;
}

bool isnan(double num) noexcept(true) {
    return num != num;
}

bool isinf(double num) noexcept(true) {
    return num == std::numeric_limits<double>::infinity()
        || num == -std::numeric_limits<double>::infinity();
}

}  // namespace Computor
