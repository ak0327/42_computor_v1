#include "Parser.hpp"
#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <sstream>

Parser::Parser() : polynomial_(), variable_() {
    // for (int i = 0; i <= this->max_degree_; ++i) {
    //     this->polynomial_[i] = 0;
    // }
}

Parser::~Parser() {}

// equation:  A0 * X^0 + A1 * X^1 + A2 * X^2 = 0
// tokens  : [A0][*][X][^][0][+][A1][*][X][^][1][+][A2][*][X][^][2][=][0]
Result<Polynomials, ErrMsg> Parser::parse_equation(
        const Tokens &tokens) noexcept(true) {
    // token -> term
    auto current = tokens.cbegin();
    auto end = tokens.cend();
    auto begin = current;

    if (tokens.empty()) {
        std::string err_msg = error_message(&current, end);
        return Result<Polynomials, ErrMsg>::err(err_msg);
    }
    parse_expression(&current, end, true);
    if (current == begin || Parser::is_at_end(&current, end)) {
        std::string err_msg = error_message(&current, end);
        return Result<Polynomials, ErrMsg>::err(err_msg);
    }
    if (!Parser::consume(&current, end, OperatorEqual)) {
        std::string err_msg = error_message(&current, end);
        return Result<Polynomials, ErrMsg>::err(err_msg);
    }

    begin = current;
    parse_expression(&current, end, false);
    if (current == begin || !Parser::is_at_end(&current, end)) {
        std::string err_msg = error_message(&current, end);
        return Result<Polynomials, ErrMsg>::err(err_msg);
    }

    // valid poly, nan, inf, etc...
    Result<Computor::Status, ErrMsg> validate_result = Parser::validate();
    if (validate_result.is_err()) {
        std::string err_msg = validate_result.err_value();
        return Result<Polynomials, ErrMsg>::err(err_msg);
    }

    Parser::reduce();
    return Result<Polynomials, ErrMsg>::ok(this->polynomial_);
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

const Polynomials &Parser::polynomial() const noexcept(true) {
    return this->polynomial_;
}

std::string Parser::reduced_form() const noexcept(true) {
    return Parser::reduced_form(this->polynomial_);
}


////////////////////////////////////////////////////////////////////////////////

Result<Computor::Status, ErrMsg> Parser::validate() noexcept(true) {
    for (auto &poly : this->polynomial_) {
        int degree = poly.first;
        double coef = poly.second;
        if (std::isnan(coef)) {
            std::string err_msg = "calculation error: coefficient is NaN at degree ";
            err_msg.append(std::to_string(degree));
            return Result<Computor::Status, ErrMsg>::err(err_msg);
        }
        if (std::isinf(coef)) {
            std::string err_msg = "calculation error: coefficient is infinity at degree ";
            err_msg.append(std::to_string(degree));
            return Result<Computor::Status, ErrMsg>::err(err_msg);
        }
    }
    return Result<Computor::Status, ErrMsg>::ok(Computor::Status::SUCCESS);
}

void Parser::reduce() noexcept(true) {
    // std::cout << "before: ";
    // Parser::display_reduced_form();
    Parser::drop_zero_term();
    Parser::adjust_sign();
    // Parser::display_polynomial();
    // std::cout << "after : ";
    // Parser::display_reduced_form();
    // std::cout << std::endl;
}

// delete term; coef == 0.0
// min poly: {0, 0}
void Parser::drop_zero_term() noexcept(true) {
    Polynomials new_poly;

    for (auto poly : this->polynomial_) {
        int pow = poly.first;
        double coef = poly.second;
        if (coef == 0.0) { continue; }
        new_poly[pow] = coef;
    }
    if (new_poly.empty()) {
        new_poly[0] = 0.0;
    }
    this->polynomial_ = new_poly;
}

// 最大次元の係数を正とするよう符号を調整
void Parser::adjust_sign() noexcept(true) {
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
        itr.second = Computor::normalize_zero(coef);
    }
}

bool Parser::is_valid_degree(int degree) const noexcept(true) {
    return (0 <= degree && degree <= this->max_degree_);
}

bool Parser::is_valid_coef(double coef) const noexcept(true) {
    std::cout << "coef: " << coef << std::endl;
    return !std::isnan(coef) && !std::isinf(coef);
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
    // if (!Parser::is_valid_coef(this->polynomial_[degree])) {  // todo: unnecessary?
    //     // std::cerr << "[Error] invalid coefficient, too large or too small" << std::endl;
    //     return Computor::Status::FAILURE;
    // }
    return Computor::Status::SUCCESS;
}

std::string Parser::error_message(
        TokenItr *current,
        const TokenItr &end) noexcept(true) {
    if (Parser::is_at_end(current, end)) {
        return "invalid equation";
    } else {
        return "syntax error: unexpected token near: " + (*current)->word;
    }
}

//  lhs                              rhs
//  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv   v
//  A0 * X^0 + A1 * X^1 + A2 * X^2 = 0
//           ^先頭以外の項は、parse前に符号評価
void Parser::parse_expression(
        TokenItr *current,
        const TokenItr &end,
        bool is_lhs) noexcept(true) {
    if (!Parser::expect(current, end, Char)
        && !Parser::expect(current, end, Integer)
        && !Parser::expect(current, end, Decimal)
        && !Parser::expect(current, end, OperatorPlus)
        && !Parser::expect(current, end, OperatorMinus)) {
        return;
    }
    while (!Parser::is_at_end(current, end)) {
        // parse term
        auto begin = *current;
        Result<s_term, Computor::Status> result = Parser::parse_term(current, end);
        if (result.is_err()) {
            *current = begin;
            return;
        }
        s_term term = result.ok_value();

        // validate term
        if (Parser::set_valid_term(term, is_lhs) == Computor::Status::FAILURE) {
            *current = begin;
            return;
        }

        if (Parser::expect(current, end, OperatorPlus)
        || Parser::expect(current, end, OperatorMinus)) {
            continue;
        }
        break;
    }
}

bool Parser::is_at_end(
        TokenItr *current,
        const TokenItr &end) noexcept(true) {
    return *current == end;
}

bool Parser::consume(
        TokenItr *current,
        const TokenItr &end,
        TokenKind expected_kind) noexcept(true) {
    if (expect(current, end, expected_kind)) {
        ++(*current);
        return true;
    }
    return false;
}

bool Parser::expect(
        TokenItr *current,
        const TokenItr &end,
        TokenKind expected_kind) noexcept(true) {
    return *current != end && (*current)->kind == expected_kind;
}

std::pair<Computor::Status, double> Parser::stod(const std::string &word) noexcept(true) {
    std::pair<Computor::Status, double> result;
    result.first = Computor::FAILURE;

    if (word.empty() || !std::isdigit(word[0])) {
        return result;
    }
    try {
        std::size_t end;
        double dnum = std::stod(word, &end);
        if (end < word.length()) {
            return result;
        }
        result.first = Computor::SUCCESS;
        result.second = dnum;
        return result;
    } catch (const std::exception &e) {
        // out of range, invalid argument
        return result;
    }
}

std::pair<Computor::Status, int> Parser::stoi(const std::string &word) noexcept(true) {
    std::pair<Computor::Status, int> result;
    result.first = Computor::FAILURE;

    if (word.empty() || !std::isdigit(word[0])) {
        return result;
    }
    try {
        std::size_t end;
        int inum = std::stoi(word, &end);
        if (end < word.length()) {
            return result;
        }
        result.first = Computor::SUCCESS;
        result.second = inum;
        return result;
    } catch (const std::exception &e) {
        // out of range, invalid argument
        return result;
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
Result<s_term, Computor::Status> Parser::parse_term(
        TokenItr *current,
        const TokenItr &end) noexcept(true) {
    s_term term = {};
    double coefficient;
    char variable = '\0';
    int degree;
    int sign = 1;

    // operator
    // +a*X^b, -aX^b, X, +a, a
    // ^       ^      ^  ^   ^ current
    if (Parser::expect(current, end, OperatorPlus) || Parser::expect(current, end, OperatorMinus)) {
        (*current)->kind == OperatorPlus ? sign = 1 : sign = -1;

        auto next = std::next(*current);
        if (!Parser::expect(&next, end, Integer)
        && !Parser::expect(&next, end, Decimal)
        && !Parser::expect(&next, end, Char)) {
            return Result<s_term, Computor::Status>::err(Computor::Status::FAILURE);
        }
        *current = next;
    }

    // coef
    // a*X^b, aX^b, X^b, aX, X, a
    // ^      ^     ^    ^   ^  ^ current
    if (Parser::expect(current, end, Integer) || Parser::expect(current, end, Decimal)) {
        // OK: aX^b, a*X^b, a
        // NG: aX^, a*X^, a*, aXX, ...
        std::pair<Computor::Status, double> result = Parser::stod((*current)->word);
        if (result.first == Computor::Status::FAILURE) {
            return Result<s_term, Computor::Status>::err(Computor::Status::FAILURE);
        }

        auto next = std::next(*current);
        if (Parser::consume(&next, end, OperatorMul) && !Parser::expect(&next, end, Char)) {
            return Result<s_term, Computor::Status>::err(Computor::Status::FAILURE);
        }
        *current = next;
        coefficient = result.second;
    } else {
        // X^b, X
        coefficient = 1.0;
    }

    // base
    // a*X^b, aX^b, X^b, aX, X, a
    //   ^     ^    ^     ^  ^   ^ current
    if (Parser::expect(current, end, Char)) {
        // X^b, X
        variable = (*current)->word[0];
        ++(*current);

        if (Parser::consume(current, end, TermPowSymbol)) {
            // X^b
            if (Parser::expect(current, end, Integer)) {
                std::pair<Computor::Status, int> result = Parser::stoi((*current)->word);
                if (result.first == Computor::Status::FAILURE) {
                    return Result<s_term, Computor::Status>::err(Computor::Status::FAILURE);
                }
                degree = result.second;
                ++(*current);
            } else {
                return Result<s_term, Computor::Status>::err(Computor::Status::FAILURE);
            }
        } else {
            // X
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
    return Result<s_term, Computor::Status>::ok(term);
}


////////////////////////////////////////////////////////////////////////////////


// 0 = 0は表示, 0 * X + 1 = 0は非表示
// ^           ^^^^^
std::string Parser::reduced_form(const Polynomials &polynomial) const noexcept(true) {
    std::ostringstream reduced_form;
    bool is_first_term = true;

    for (auto itr = polynomial.crbegin(); itr != polynomial.crend(); ++itr) {
        int pow = itr->first;
        double coef = itr->second;
        std::string sign = "";

        if (coef == 0.0) { continue; }
        if (coef < 0) {
            sign = "- ";
        } else if (0 < coef && !is_first_term) {
            sign = "+ ";
        }

        // reduced_form << sign << std::fixed << std::setprecision(2) << std::abs(coef);
        reduced_form << sign << std::abs(coef);
        if (pow == 1) {
            reduced_form << " * " << this->variable_;
        } else if (1 < pow) {
            reduced_form << " * " << this->variable_ << "^" << pow;
        }
        reduced_form << " ";

        if (is_first_term) { is_first_term = false; }
    }
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
