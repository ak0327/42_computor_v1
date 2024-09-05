#include "computor.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

const char SP        = ' ';
const char OP_PLUS   = '+';
const char OP_MINUS  = '-';
const char OP_MUL    = '*';
const char OP_EQUAL  = '=';
const char OP_POW    = '^';


int Computor::calc_equation(const std::string &equation) noexcept(true) {
    Tokenizer tokenizer;
    if (tokenizer.tokenize(equation) == Status::FAILURE) {
        return EXIT_FAILURE;
    }
    std::deque<s_token> tokens = tokenizer.tokens();

    Parser parser;
    if (parser.parse_equation(tokens) == Status::FAILURE) {
        return EXIT_FAILURE;
    }
    std::map<int, double> polynomial = parser.get_polynomial();

    // display calc info

    // calc

    return EXIT_SUCCESS;
}
