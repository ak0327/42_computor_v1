#pragma once

#include <deque>
#include <string>
#include <map>
#include "computor.hpp"
#include "Tokenizer.hpp"

struct s_term {
    double  coefficient;
    char    variable;
    int     degree;
};

class Parser {
 public:
    Parser();
    ~Parser();

    Status parse_equation(const std::deque<s_token> &tokens) noexcept(true);
    void parse_expression(
            const std::deque<s_token> &tokens,
            std::deque<s_token>::const_iterator *itr,
            bool is_lhs) noexcept(true);
    void display_reduced_form() const noexcept(true);
    void display_polynomial_degree() const noexcept(true);
    std::map<int, double> polynomial() const noexcept(true);

    bool is_valid_degree(int degree) const noexcept(true);
    bool is_valid_coef(int degree) const noexcept(true);
    bool is_valid_variable(char var, int degree) const noexcept(true);

    static s_term parse_term(
            const std::deque<s_token> &tokens,
            std::deque<s_token>::const_iterator *itr) noexcept(true);
    Status set_valid_term(const s_term &term, bool is_lhs) noexcept(true);

    static void skip_sp(
            const std::string &str,
            std::size_t start_pos,
            std::size_t *end_pos) noexcept(true);

 private:
    int max_degree_ = 2;
    std::map<int, double> polynomial_;
    char variable_;

    // copy invalid
    Parser &operator=(const Parser &rhs);
    explicit Parser(const Parser &other);

    Status set_variable(char var, int degree);
};

std::ostream &operator<<(std::ostream &out, const s_term &term);
