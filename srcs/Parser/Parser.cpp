#include "Parser.hpp"
#include <climits>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <deque>

Parser::Parser() : polynomial_(), variable_() {
    // for (int i = 0; i <= this->max_degree_; ++i) {
    //     this->polynomial_[i] = 0;
    // }
}

Parser::~Parser() {}

//  A0 * X^0 + A1 * X^1 + A2 * X^2 = 0
Computor::Status Parser::parse_equation(
        const std::deque<s_token> &tokens) noexcept(true) {
    // token -> term
    if (tokens.empty()) {
        return Computor::Status::FAILURE;
    }
    auto itr = tokens.begin();
    auto itr_copy = itr;
    parse_expression(tokens, &itr, true);
    if (itr == tokens.end() || itr->kind != OperatorEqual || itr == itr_copy) {
        return Computor::Status::FAILURE;
    }
    ++itr;
    if (itr == tokens.end()) {
        return Computor::Status::FAILURE;
    }

    // "=" ?
    itr_copy = itr;
    parse_expression(tokens, &itr, false);
    if (itr != tokens.end() || itr == itr_copy) {
        return Computor::Status::FAILURE;
    }

    // std::cout << "before: ";
    // Parser::display_reduced_form();
    Parser::adjust_equation_sign();
    // Parser::display_polynomial();
    // std::cout << "after : ";
    // Parser::display_reduced_form();
    // std::cout << std::endl;
    return Computor::Status::SUCCESS;
}

void Parser::display_reduced_form() const noexcept(true) {
    std::cout << "Reduced form     : " << Parser::reduced_form() << std::endl;
}

void Parser::display_polynomial_degree() const noexcept(true) {
    auto itr = this->polynomial_.crbegin();
    if (itr == this->polynomial_.crend()) {
        return;
    }
    int max_degree = itr->first;
    std::cout << "Polynomial degree: " << max_degree << std::endl;
}

std::map<int, double> Parser::polynomial() const noexcept(true) {
    return this->polynomial_;
}

std::string Parser::reduced_form() const noexcept(true) {
    return Parser::reduced_form(this->polynomial_);
}


////////////////////////////////////////////////////////////////////////////////


// 最大次元の係数を正とするよう符号を調整
void Parser::adjust_equation_sign() noexcept(true) {
    auto highest_degree_term = this->polynomial_.crbegin();
    if (highest_degree_term == this->polynomial_.crend()) {
        return;
    }
    for (auto itr = this->polynomial_.crbegin(); itr != this->polynomial_.crend(); ++itr) {
        int pow = itr->first;
        double coef = itr->second;

        if (pow == 0 && coef < 0) { break; }
        if (coef == 0.0) { continue; }
        if (0 < coef) { return; }
        break;
    }

    // std::cout << " adjust sign -> *= -1" << std::endl;
    for (auto &itr : this->polynomial_) {
        double coef = itr.second * -1;
        itr.second = normalize_zero(coef);
    }
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

Computor::Status Parser::set_variable(char var, int degree) {
    if (degree == 0 || this->variable_ != '\0') {
        return Computor::Status::SUCCESS;
    }
    if (var != 0) {
        this->variable_ = var;
        return Computor::Status::SUCCESS;
    }
    return Computor::Status::FAILURE;
}

// todo degreeの判定はあとで
// var
Computor::Status Parser::set_valid_term(const s_term &term, bool is_lhs) noexcept(true) {
    int degree = term.degree;
    char var = term.variable;
    double coef = term.coefficient;

    // if (!Parser::is_valid_degree(degree)) {
    //     return Computor::Status::FAILURE;
    // }
    if (Parser::set_variable(var, degree) == Computor::Status::FAILURE) {
        return Computor::Status::FAILURE;
    }
    if (!Parser::is_valid_variable(var, degree)) {
        return Computor::Status::FAILURE;
    }
    this->polynomial_[degree] += (is_lhs ? 1 : -1) * coef;
    if (!Parser::is_valid_coef(degree)) {
        return Computor::Status::FAILURE;
    }
    return Computor::Status::SUCCESS;
}


//  lhs                              rhs
//  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv   v
//  A0 * X^0 + A1 * X^1 + A2 * X^2 = 0
//           ^先頭以外の項は、parse前に符号評価
void Parser::parse_expression(
        const std::deque<s_token> &tokens,
        std::deque<s_token>::const_iterator *itr,
        bool is_lhs) noexcept(true) {
    if (*itr == tokens.end() || (*itr)->kind == OperatorEqual) {
        return;
    }
    while (*itr != tokens.end()) {
        // parse term
        s_term term = Parser::parse_term(tokens, itr);
        // std::cout << term << std::endl;

        // validate term
        if (Parser::set_valid_term(term, is_lhs) == Computor::Status::FAILURE) {
            --(*itr);  // todo:
            return;
        }

        if (*itr != tokens.end()
        && ((*itr)->kind == OperatorPlus || (*itr)->kind == OperatorMinus)) {
            continue;
        }
        break;
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
        // std::cout << "1 " << (*itr)->word << std::endl;
        ++(*itr);
    }
    // coef
    if (*itr != tokens.end() && (*itr)->kind == TermCoef) {
        // std::cout << "2 " << (*itr)->word << std::endl;
        try {
            coefficient = std::stod((*itr)->word, &end);
            if (end < (*itr)->word.length()) {
                // std::cout << "3" << std::endl;
                return term;
            }
            ++(*itr);

            if (*itr != tokens.end() && (*itr)->kind == OperatorMul) {
                ++(*itr);
                if ((*itr)->kind != TermBase) {
                    // todo: error
                    // std::cout << "4" << std::endl;
                    return term;
                }
                // std::cout << "5" << std::endl;
            }
        } catch (const std::exception &e) {
            // std::cout << "6" << std::endl;
            // out of range, invalid argument
            return term;
        }
    } else {
        // std::cout << "7" << std::endl;
        coefficient = 1.0;
    }

    if (*itr != tokens.end() && (*itr)->kind == TermBase) {
        // var
        // std::cout << "8 " << (*itr)->word << std::endl;
        variable = (*itr)->word[0];
        ++(*itr);

        // pow
        if (*itr != tokens.end() && (*itr)->kind == TermPowSymbol) {
            // std::cout << "9 " << (*itr)->word << std::endl;
            ++(*itr);

            if (*itr != tokens.end() && (*itr)->kind == TermPower) {
                degree = std::stoi((*itr)->word, &end);
                // std::cout << "10 " << (*itr)->word << std::endl;
                if (end < (*itr)->word.length()) {
                    return term;
                }
                ++(*itr);
            } else {
                // std::cout << "11" << std::endl;
                // todo: error
                return term;
            }
            // std::cout << "12" << std::endl;
        } else {
            // std::cout << "13" << std::endl;
            degree = 1;
        }
    } else {
        // std::cout << "14" << std::endl;
        degree = 0;
    }
    // std::cout << "15" << std::endl;

    term = {
            .coefficient = coefficient * sign,
            .variable = variable,
            .degree = degree
    };
    return term;
}


////////////////////////////////////////////////////////////////////////////////


// 0 = 0は表示, 0 * X + 1 = 0は非表示
// ^           ^^^^^
std::string Parser::reduced_form(const std::map<int, double> &polynomial) const noexcept(true) {
    std::ostringstream reduced_form;
    bool is_first_term = true;

    for (auto itr = polynomial.crbegin(); itr != polynomial.crend(); ++itr) {
        int pow = itr->first;
        double coef = itr->second;
        std::string sign = "";

        // std::cout << "pow=" << pow
        // << ", coef=" << coef << ", var=" << this->variable_ << std::endl;
        if (coef == 0.0) { continue; }
        if (coef < 0) {
            sign = "- ";
        } else if (0 < coef && !is_first_term) {
            sign = "+ ";
        }
        // std::cout << "2" << std::endl;

        // reduced_form << sign << std::fixed << std::setprecision(2) << std::abs(coef);
        reduced_form << sign << std::abs(coef);
        if (pow == 1) {
            // std::cout << "3" << std::endl;
            reduced_form << " * " << this->variable_;
        } else if (1 < pow) {
            // std::cout << "4" << std::endl;
            reduced_form << " * " << this->variable_ << "^" << pow;
        }
        reduced_form << " ";

        // std::cout << "5" << std::endl;
        if (is_first_term) { is_first_term = false; }
    }
    // std::cout << "6" << std::endl;
    if (reduced_form.str().empty()) {
        reduced_form << "0 ";
    }
    reduced_form << "= 0";
    return reduced_form.str();
}


////////////////////////////////////////////////////////////////////////////////


void Parser::display_polynomial() const noexcept(true) {
    for (auto itr = this->polynomial_.crbegin(); itr != this->polynomial_.crend(); ++itr) {
        int pow = itr->first;
        double coef = itr->second;

        std::cout << "[" << pow << "]: " << coef << "" << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////


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

std::ostream &operator<<(std::ostream &out, const s_term &term) {
    out << "term [ coef: " << term.coefficient
    << ", var: " << term.variable
    << ", degree: " << term.degree << " ]";
    return out;
}

double normalize_zero(double value) noexcept(true) {
    return value == 0.0 ? 0.0 : value;
}
