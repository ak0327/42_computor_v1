#pragma once

#include "computor.hpp"
#include <string>
#include <deque>

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

class Tokenizer {
 public:
    Tokenizer();
    ~Tokenizer();

    Status tokenize(const std::string &equation) noexcept(true);
    Status tagging(const std::deque<std::string> &split) noexcept(true);
    void init_tokens(const std::deque<std::string> &split) noexcept(true);
    void tagging_operators() noexcept(true);
    void tagging_terms() noexcept(true);
    Status validate_tokens() const noexcept(true);
    static std::deque<s_token> split_term_coef_and_base(const std::deque<s_token> &tokens) noexcept(true);
    static std::deque<std::string> split_equation(const std::string &equation) noexcept(true);

    static std::deque<std::string> split_by_delimiter(
            const std::string &src,
            char delimiter,
            bool keep_delimiter = false) noexcept(true);

    static std::deque<std::string> split_by_delimiter(
            const std::deque<std::string> &src,
            char delimiter,
            bool keep_delimiter = false) noexcept(true);

    static bool is_term_base(const std::string &str) noexcept(true);
    static bool is_term_coef(const std::string &str, TokenKind prev_kind, TokenKind next_kind) noexcept(true);
    static bool is_term_pow(const std::string &str, TokenKind prev_kind, TokenKind next_kind) noexcept(true);

    static bool is_num(const std::string &str) noexcept(true);

    std::deque<s_token> tokens() noexcept(true);

 private:
    std::deque<s_token> tokens_;

    // copy invalid
    Tokenizer &operator=(const Tokenizer &rhs);
    explicit Tokenizer(const Tokenizer &other);

};
