#pragma once
#include <string>

namespace Computor {

enum Status {
    SUCCESS = 1,
    FAILURE = 0,
};

extern const char SP;        // ' '
extern const char OP_PLUS;   // '+'
extern const char OP_MINUS;  // '-'
extern const char OP_MUL;    // '*'
extern const char OP_EQUAL;  // '='
extern const char OP_POW;    // '^'

int calc_equation(const std::string &equation) noexcept(true);
double normalize_zero(double value) noexcept(true);

}  // namespace Computor
