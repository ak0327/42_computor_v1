#pragma once

#include <deque>
#include <map>
#include <string>
#include <utility>
#include "computor.hpp"
#include "Tokenizer.hpp"

struct s_term {
    double  coefficient;
    char    variable;
    int     degree;
};


typedef std::map<int, double> Polynomials;


class Parser {
 public:
    Parser();
    ~Parser();

    Result<Polynomials, ErrMsg> parse_equation(const std::deque<s_token> &tokens) noexcept(true);
    void display_reduced_form() const noexcept(true);
    void display_polynomial_degree() const noexcept(true);

    const std::map<int, double> &polynomial() const noexcept(true);
    std::string reduced_form() const noexcept(true);

    friend class TestParser;

 private:
    int max_degree_ = 2;
    std::map<int, double> polynomial_;
    char variable_;

    void parse_expression(
            std::deque<s_token>::const_iterator *itr,
            const std::deque<s_token>::const_iterator &end,
            bool is_lhs) noexcept(true);
    void adjust_equation_sign() noexcept(true);
    std::string reduced_form(const std::map<int, double> &polynomial) const noexcept(true);
    void display_polynomial() const noexcept(true);

    bool is_valid_degree(int degree) const noexcept(true);
    bool is_valid_coef(int degree) const noexcept(true);
    bool is_valid_variable(char var, int degree) const noexcept(true);

    static s_term parse_term(
            std::deque<s_token>::const_iterator *current,
            const std::deque<s_token>::const_iterator &end) noexcept(true);

    Computor::Status set_valid_term(const s_term &term, bool is_lhs) noexcept(true);

    static std::pair<Computor::Status, double> stod(const std::string &word) noexcept(true);
    static std::pair<Computor::Status, int> stoi(const std::string &word) noexcept(true);

    static bool is_at_end(
            std::deque<s_token>::const_iterator *current,
            const std::deque<s_token>::const_iterator &end) noexcept(true);

    static bool consume(
            std::deque<s_token>::const_iterator *current,
            const std::deque<s_token>::const_iterator &end,
            TokenKind expected_kind) noexcept(true);

    static bool expect(
            std::deque<s_token>::const_iterator *current,
            const std::deque<s_token>::const_iterator &end,
            TokenKind expected_kind) noexcept(true);

    static void skip_sp(
            const std::string &str,
            std::size_t start_pos,
            std::size_t *end_pos) noexcept(true);

    static std::string error_message(
            std::deque<s_token>::const_iterator *current,
            const std::deque<s_token>::const_iterator &end) noexcept(true);

    // copy invalid
    Parser &operator=(const Parser &rhs);
    explicit Parser(const Parser &other);

    Computor::Status set_variable(char var, int degree);
};

std::ostream &operator<<(std::ostream &out, const s_term &term);
