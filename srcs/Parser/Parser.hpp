#pragma once

#include "computor.hpp"
#include <string>
#include <map>

struct s_term {
    char    variable;
    int     coefficient;
    int     degree;
};

class Parser {
 public:
    Parser();
    ~Parser();

    Status parse_equation(const std::string &equation) noexcept(true);
    Status validate_polynomial(int degree = 3) const noexcept(true);

    std::map<int, int> get_polynomial() const noexcept(true);

    Status parse_expression(const std::string &expression, bool is_lhs) noexcept(true);
    s_term parse_term(
            const std::string &expr,
            std::size_t start_pos,
            std::size_t *end_pos) noexcept(true);

    static int stoi(const std::string &str,
                    std::size_t start_pos,
                    std::size_t *end_pos) noexcept(true);
    static void skip_sp(const std::string &str,
                        std::size_t start_pos,
                        std::size_t *end_pos) noexcept(true);

 private:
    std::map<int, int> polynomial_;
    // char variable_;
};
