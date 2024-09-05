#pragma once

#include "computor.hpp"
#include <string>
#include <map>

struct s_term {
    int     coefficient;
    char    variable;
    int     degree;
};

class Parser {
 public:
    Parser();
    ~Parser();

    Status parse_equation(const std::string &equation) noexcept(true);
    std::map<int, long> get_polynomial() const noexcept(true);

    Status parse_expression(const std::string &expression, bool is_lhs) noexcept(true);

    bool is_valid_degree(int degree) const noexcept(true);
    bool is_valid_coef(int degree) const noexcept(true);
    bool is_valid_variable(char var, int degree) const noexcept(true);

    static s_term parse_term(
            const std::string &expr,
            std::size_t start_pos,
            std::size_t *end_pos) noexcept(true);

    static int stoi(
            const std::string &str,
            std::size_t start_pos,
            std::size_t *end_pos,
            std::size_t *num_len = nullptr) noexcept(true);
    static void skip_sp(
            const std::string &str,
            std::size_t start_pos,
            std::size_t *end_pos) noexcept(true);

 private:
    int max_degree_ = 2;
    std::map<int, long> polynomial_;
    char variable_;

    // copy invalid
    Parser &operator=(const Parser &rhs);
    explicit Parser(const Parser &other);

    Status set_variable(char var, int degree);
};
