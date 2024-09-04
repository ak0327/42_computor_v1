#include "Parser.hpp"

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


Parser::Parser() {}

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

//  A0 * X^0 + A1 * X^1 + A2 * X^2 = 0
// ^ 先頭は符号なくてもOK
Status Parser::parse_expression(const std::string &expression, bool is_lhs) noexcept(true) {
    (void)expression;
    (void)is_lhs;

    // 先頭に符号をつける？

    // variable格納

    // 係数を格納(is_lhsの場合は+, !is_lhsの場合は-で加算)

    return Status::SUCCESS;
}

s_term Parser::parse_term(const std::string &expr,
                          std::size_t start_pos,
                          std::size_t *end_pos) noexcept(true) {
    s_term term;
    (void)expr;
    (void)start_pos;
    (void)end_pos;

    return term;
}

Status Parser::validate_polynomial(int degree) const noexcept(true) {
    (void)degree;

    return Status::SUCCESS;
}

std::map<int, int> Parser::get_polynomial() const noexcept(true) {
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
                 std::size_t *end_pos) noexcept(true) {
    int num = 0;

    if (end_pos) { *end_pos = start_pos; }
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
    if (end_pos && i != 0) { *end_pos = pos + i; }
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
