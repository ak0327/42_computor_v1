#include "Parser.hpp"
#include <iostream>

namespace {

bool is_in_int_range(int before_x10_num,
                     int add_num) {
    int max_div, max_mod;

    if (before_x10_num == INT_MAX || before_x10_num == INT_MIN) {
        return false;
    }
    if (before_x10_num > 0) {
        max_div = INT_MAX / 10;
        max_mod = INT_MAX % 10;
    } else {
        max_div = -(INT_MIN / 10);
        max_mod = -(INT_MIN % 10);
    }
    if (std::abs(before_x10_num) < max_div) {
        return true;
    }
    if (std::abs(before_x10_num) == max_div && max_mod == add_num) {
        return true;
    }
    return false;
}


int to_digit(char c) {
    if ('0' <= c and c <= '9') {
        return (c - '0');
    }
    return c;
}


}  // namespace


Parser::Parser() : polynomial_(), variable_() {
    for (int i = 0; i <= this->max_degree_; ++i) {
        this->polynomial_[i] = 0;
    }
}

Parser::~Parser() {}

Status Parser::parse_equation(const std::string &equation) noexcept(true) {
    std::size_t equals_pos = equation.find('=');
    if (equals_pos == std::string::npos) {
        return Status::FAILURE;
    }

    std::string lhs = equation.substr(0, equals_pos);
    if (Parser::parse_expression(lhs, true) == Status::FAILURE) {
        return Status::FAILURE;
    }

    std::string rhs = equation.substr(equals_pos + 1);
    if (Parser::parse_expression(rhs, false) == Status::FAILURE) {
        return Status::FAILURE;
    }
    return Status::SUCCESS;
}

bool Parser::is_valid_degree(int degree) const noexcept(true) {
    return (0 <= degree && degree <= this->max_degree_);
}

bool Parser::is_valid_coef(int degree) const noexcept(true) {
    try {
        int coef = this->polynomial_.at(degree);
        return (INT_MIN <= coef && coef <= INT_MAX);
    } catch (const std::out_of_range &e) {}
    return false;
}

bool Parser::is_valid_variable(char var, int degree) const noexcept(true) {
    if (degree == 0) {
        return true;
    }
    return this->variable_ == var;
}

Status Parser::set_variable(char var, int degree) {
    if (degree == 0 || this->variable_ != '\0') {
        return Status::SUCCESS;
    }
    if (var != 0) {
        this->variable_ = var;
        return Status::SUCCESS;
    }
    return Status::FAILURE;
}

//  lhs                              rhs
//  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv   v
//  A0 * X^0 + A1 * X^1 + A2 * X^2 = 0
//           ^先頭以外の項は、parse前に符号評価
Status Parser::parse_expression(const std::string &expression, bool is_lhs) noexcept(true) {
    if (expression.empty()) {
        return Status::FAILURE;
    }

    std::size_t pos, end;
    pos = 0;
    while (expression[pos]) {
        s_term term = Parser::parse_term(expression, pos, &end);
        if (pos == end) {
            break;
        }

        int degree = term.degree;
        char var = term.variable;
        int coef = term.coefficient;

        if (!Parser::is_valid_degree(degree)) {
            return Status::FAILURE;
        }
        if (Parser::set_variable(var, degree) == Status::FAILURE) {
            return Status::FAILURE;
        }
        if (!Parser::is_valid_variable(var, degree)) {
            return Status::FAILURE;
        }
        this->polynomial_[degree] += (is_lhs ? 1 : -1) * coef;
        if (!Parser::is_valid_coef(degree)) {
            return Status::FAILURE;
        }
        if (!(expression[end] == '+' || expression[end] == '-' || !expression[end])) {
            // next term invalid
            break;
        }
        pos = end;
    }

    if (pos < expression.length()) {
        return Status::FAILURE;
    }
    return Status::SUCCESS;
}

// term = ( sign ) ( coefficient "*" ) ALPHA "^" 1*( DIGIT )
// ok +X -X
// ng 1X *X
s_term Parser::parse_term(const std::string &expr,
                          std::size_t start_pos,
                          std::size_t *end_pos) noexcept(true) {
    s_term term = {};
    char variable;
    int coefficient, degree;

    std::size_t pos, end, num_len;

    *end_pos = start_pos;
    pos = start_pos;
    coefficient = Parser::stoi(expr, pos, &end, &num_len);
    if (pos == end) {
        coefficient = 1;
    } else {
        pos = end;
        Parser::skip_sp(expr, pos, &pos);
        if (0 < num_len && expr[pos] != '*') { return term; }
        if (0 < num_len && expr[pos] == '*') { ++pos; }
    }
    Parser::skip_sp(expr, pos, &pos);

    if (!std::isalpha(expr[pos])) { return term; }
    variable = expr[pos];
    ++pos;

    if (expr[pos] != '^') { return term; }
    ++pos;

    if (!std::isdigit(expr[pos])) { return term; }
    degree = Parser::stoi(expr, pos, &end);
    pos = end;

    Parser::skip_sp(expr, pos, &end);
    *end_pos = end;

    term.coefficient = coefficient;
    term.variable = variable;
    term.degree = degree;
    return term;
}

std::map<int, long> Parser::get_polynomial() const noexcept(true) {
    return this->polynomial_;
}

/*
 equation    = 1*expression "=" 1*expression
 expression  = 1*[ *(SP) term *(SP) ]
 term        = ( sign ) ( coefficient "*" ) ALPHA "^" 1*( DIGIT )
 sign        = "+" / "-"
 coefficient = 1*DIGIT
 ALPHA       = A-Z / a-z
 DIGIT       = 0-9
 SP          = " "
 */

//  term = ( sign ) ( coefficient "*" ) ALPHA "^" 1*( DIGIT )
//         ^^^^^^^^^^^^^^^^^^^^^^ stoi
int Parser::stoi(const std::string &str,
                 std::size_t start_pos,
                 std::size_t *end_pos,
                 std::size_t *num_len) noexcept(true) {
    int num = 0;

    if (end_pos) { *end_pos = start_pos; }
    if (num_len) { *num_len = 0; }
    if (str.length() <= start_pos) { return num; }

    std::size_t pos, i;
    Parser::skip_sp(str, start_pos, &pos);
    int sign = 1;
    if (str[pos] == '+' || str[pos] == '-') {
        if (str[pos] == '-') { sign = -1; }
        ++pos;
    }
    Parser::skip_sp(str, pos, &pos);

    num = 0;
    int digit;
    i = 0;
    while (std::isdigit(str[pos + i])) {
        digit = to_digit(str[pos + i]);
        if (!is_in_int_range(num, digit)) {
            return num;
        }
        num = num * 10 + sign * digit;
        ++i;
    }
    if (end_pos) { *end_pos = pos + i; }
    if (num_len) { *num_len = i; }
    if (i == 0) {
        // '+X'-> +1, '-X' -> -1
        num = sign;
    }
    return num;
}

void Parser::skip_sp(const std::string &str,
                     std::size_t start_pos,
                     std::size_t *end_pos) noexcept(true) {
    std::size_t pos;

    if (!end_pos) {
        return;
    }
    *end_pos = start_pos;
    if (str.empty() || str.length() <= start_pos) {
        return;
    }
    pos = start_pos;
    while (str[pos] && str[pos] == ' ') {
        ++pos;
    }
    *end_pos = pos;
}
