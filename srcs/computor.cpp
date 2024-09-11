#include "computor.hpp"
#include <iostream>
#include "Calculator.hpp"
#include "Tokenizer.hpp"
#include "Result.hpp"
#include "Parser.hpp"

namespace Computor {

const char SP        = ' ';
const char OP_PLUS   = '+';
const char OP_MINUS  = '-';
const char OP_MUL    = '*';
const char OP_EQUAL  = '=';
const char OP_POW    = '^';


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

}  // namespace Computor
