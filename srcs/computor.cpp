#include "computor.hpp"
#include <deque>
#include <map>
#include "Calculator.hpp"
#include "Tokenizer.hpp"
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
    if (tokenizer.tokenize(equation) == Status::FAILURE) {
        return EXIT_FAILURE;
    }
    std::deque<s_token> tokens = tokenizer.tokens();

    Parser parser;
    if (parser.parse_equation(tokens) == Status::FAILURE) {
        return EXIT_FAILURE;
    }
    parser.display_reduced_form();
    parser.display_polynomial_degree();
    std::map<int, double> polynomial = parser.polynomial();

    Calculator calculator(polynomial);
    calculator.solve_quadratic_equation();
    return EXIT_SUCCESS;
}

double normalize_zero(double value) noexcept(true) {
    return value == 0.0 ? 0.0 : value;
}

}  // namespace Computor
