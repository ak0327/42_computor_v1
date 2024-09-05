#pragma once

#include <string>

enum Status {
    SUCCESS = 1,
    FAILURE = 0,
};

extern const char SP;
extern const char OP_PLUS;
extern const char OP_MINUS;
extern const char OP_MUL;
extern const char OP_EQUAL;
extern const char OP_POW;


namespace Computor {


int calc_equation(const std::string &equation) noexcept(true);


}  // namespace Computor
