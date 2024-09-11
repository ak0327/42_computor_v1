#pragma once

# include <deque>
# include <map>
# include <string>
# include <utility>
# include "computor.hpp"
# include "Tokenizer.hpp"

struct s_term {
    double  coefficient;
    char    variable;
    int     degree;
};


typedef std::map<int, double> Polynomials;
typedef Tokens::const_iterator TokenItr;

class Parser {
 public:
    Parser();
    ~Parser();

    Result<Polynomials, ErrMsg> parse_equation(const Tokens &tokens) noexcept(true);
    void display_reduced_form() const noexcept(true);
    void display_polynomial_degree() const noexcept(true);

    const Polynomials &polynomial() const noexcept(true);
    std::string reduced_form() const noexcept(true);

    friend class TestParser;

 private:
    int max_degree_ = 2;
    Polynomials polynomial_;
    char variable_;

    void parse_expression(
            TokenItr *itr,
            const TokenItr &end,
            bool is_lhs) noexcept(true);
    void reduce() noexcept(true);
    void drop_zero_term() noexcept(true);
    void adjust_sign() noexcept(true);
    std::string reduced_form(const Polynomials &polynomial) const noexcept(true);
    void display_polynomial() const noexcept(true);

    bool is_valid_degree(int degree) const noexcept(true);
    bool is_valid_coef(double coef) const noexcept(true);
    bool is_valid_variable(char var, int degree) const noexcept(true);

    static Result<s_term, Computor::Status> parse_term(
            TokenItr *current,
            const TokenItr &end) noexcept(true);

    Computor::Status set_valid_term(const s_term &term, bool is_lhs) noexcept(true);
    Result<Computor::Status, ErrMsg> validate() noexcept(true);

    static std::pair<Computor::Status, double> stod(const std::string &word) noexcept(true);
    static std::pair<Computor::Status, int> stoi(const std::string &word) noexcept(true);

    static bool is_at_end(
            TokenItr *current,
            const TokenItr &end) noexcept(true);

    static bool consume(
            TokenItr *current,
            const TokenItr &end,
            TokenKind expected_kind) noexcept(true);

    static bool expect(
            TokenItr *current,
            const TokenItr &end,
            TokenKind expected_kind) noexcept(true);

    static void skip_sp(
            const std::string &str,
            std::size_t start_pos,
            std::size_t *end_pos) noexcept(true);

    static std::string error_message(
            TokenItr *current,
            const TokenItr &end) noexcept(true);

    // copy invalid
    Parser &operator=(const Parser &rhs);
    explicit Parser(const Parser &other);

    Computor::Status set_variable(char var, int degree);
};

std::ostream &operator<<(std::ostream &out, const s_term &term);
