#include "Parser.hpp"
#include <iostream>

Parser::Parser() : polynomial_(), variable_() {
    for (int i = 0; i <= this->max_degree_; ++i) {
        this->polynomial_[i] = 0;
    }
}

Parser::~Parser() {}

//  A0 * X^0 + A1 * X^1 + A2 * X^2 = 0
Status Parser::parse_equation(const std::deque<s_token> &tokens) noexcept(true) {
    // token -> term
    if (tokens.empty()) {
        return Status::FAILURE;
    }

    auto itr = tokens.begin();
    parse_expression(tokens, &itr, true);
    if (itr == tokens.end() || itr->kind != OperatorEqual) {
        return Status::FAILURE;
    }
    ++itr;
    if (itr == tokens.end()) {
        return Status::FAILURE;
    }

    // "=" ?
    parse_expression(tokens, &itr, false);
    if (itr != tokens.end()) {
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

Status Parser::set_valid_term(const s_term &term, bool is_lhs) noexcept(true) {
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
    return Status::SUCCESS;
}


//  lhs                              rhs
//  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv   v
//  A0 * X^0 + A1 * X^1 + A2 * X^2 = 0
//           ^先頭以外の項は、parse前に符号評価
void Parser::parse_expression(
        const std::deque<s_token> &tokens,
        std::deque<s_token>::const_iterator *itr,
        bool is_lhs) noexcept(true) {

    while (*itr != tokens.end()) {
        // parse term
        s_term term = Parser::parse_term(tokens, itr);

        // validate term
        if (Parser::set_valid_term(term, is_lhs) == Status::FAILURE) {
            --(*itr);  // todo:
            return;
        }

        if (*itr == tokens.end()) {
            break;
        }
        auto next = (*itr)++;
        if (next == tokens.end() && (*itr)->kind != OperatorPlus && (*itr)->kind != OperatorMinus) {
            ++(*itr);
        } else {
            break;
        }
    }
}

/*
    enum TokenKind {
        None = 0,
        TermCoef,       // aX^b a
        TermBase,       // aX^b X
        TermPowSymbol,  // aX^b ^
        TermPower,      // aX^b b
        OperatorPlus,   // +
        OperatorMinus,  // -
        OperatorMul,    // *
        OperatorEqual,  // =
    };

    struct s_token {
        std::string word;
        TokenKind kind;
    };
 */
// term = ( operator ) [ coefficient ("*") ] ALPHA "^" 1*( DIGIT )
s_term Parser::parse_term(
        const std::deque<s_token> &tokens,
        std::deque<s_token>::const_iterator *itr) noexcept(true) {
    s_term term = {};
    double coefficient;
    char variable = '\0';
    int degree;
    int sign = 1;
    std::size_t end;

    // operator
    if ((*itr)->kind == OperatorPlus || (*itr)->kind == OperatorMinus) {
        (*itr)->kind == OperatorPlus ? sign = 1 : sign = -1;
        ++(*itr);
    }
    // coef
    if (*itr != tokens.end() && (*itr)->kind == TermCoef) {
        try {
            coefficient = std::stod((*itr)->word, &end);
            if (end < (*itr)->word.length()) {
                return term;
            }
            ++(*itr);

            if (*itr != tokens.end() && (*itr)->kind == OperatorMul) {
                ++(*itr);
                if ((*itr)->kind != TermBase) {
                    // todo: error
                    return term;
                }
            }
        } catch (const std::exception &e) {
            // out of range, invalid argument
            return term;
        }
    } else {
        coefficient = 1.0;
    }

    if (*itr != tokens.end() && (*itr)->kind == TermBase) {
        // var
        variable = (*itr)->word[0];
        ++(*itr);

        // pow
        if (*itr != tokens.end() && (*itr)->kind == TermPowSymbol) {
            ++(*itr);

            if (*itr != tokens.end() && (*itr)->kind == TermPower) {
                degree = std::stoi((*itr)->word, &end);
                if (end < (*itr)->word.length()) {
                    return term;
                }
                ++(*itr);
            } else {
                // todo: error
                return term;
            }
        } else {
            degree = 1;
        }
    } else {
        degree = 0;
    }

    term = {
            .coefficient = coefficient * sign,
            .variable = variable,
            .degree = degree
    };
    return term;
}

std::map<int, double> Parser::polynomial() const noexcept(true) {
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
