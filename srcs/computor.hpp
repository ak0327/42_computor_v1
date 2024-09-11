#pragma once

# include <string>

using ErrMsg = std::string;

namespace Computor {

enum Status {
    SUCCESS = 1,
    FAILURE = 0,
};

constexpr char SP       = ' ';
constexpr char OP_PLUS  = '+';
constexpr char OP_MINUS = '-';
constexpr char OP_MUL   = '*';
constexpr char OP_EQUAL = '=';
constexpr char OP_POW   = '^';

int calc_equation(const std::string &equation) noexcept(true);
double normalize_zero(double value) noexcept(true);

}  // namespace Computor
