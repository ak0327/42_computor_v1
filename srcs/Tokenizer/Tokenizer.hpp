#pragma once

# include <deque>
# include <string>
# include "computor.hpp"
# include "Result.hpp"


enum TokenKind {
    None = 0,
    Char,
    Integer,
    Decimal,
    TermPowSymbol,  // ^
    OperatorPlus,   // +
    OperatorMinus,  // -
    OperatorMul,    // *
    OperatorEqual,  // =
};

struct s_token {
    std::string word;
    TokenKind kind;
};


using Tokens = std::deque<s_token>;


class Tokenizer {
 public:
    Tokenizer();
    ~Tokenizer();

    Result<Tokens, ErrMsg> tokenize(const std::string &equation) noexcept(true);
    const Tokens &tokens() noexcept(true);

    friend class TestTokenizer;

 private:
    Tokens tokens_;

    // split
    static std::deque<std::string> split_equation(
            const std::string &equation) noexcept(true);
    static std::deque<std::string> split_by_delimiter(
            const std::string &src,
            char delimiter,
            bool keep_delimiter = false) noexcept(true);
    static std::deque<std::string> split_by_delimiter(
            const std::deque<std::string> &src,
            char delimiter,
            bool keep_delimiter = false) noexcept(true);
    static Tokens split_coef_and_base(const Tokens &tokens) noexcept(true);

    // tag
    static Tokens tagging(const std::deque<std::string> &split) noexcept(true);
    static Tokens init_tokens(const std::deque<std::string> &split) noexcept(true);
    static void tagging_operators(Tokens *tokens) noexcept(true);
    static void tagging_terms(Tokens *tokens) noexcept(true);

    static bool is_char(const std::string &str) noexcept(true);
    static bool is_integer(const std::string &str) noexcept(true);
    static bool is_decimal(const std::string &str) noexcept(true);

    // validate
    Result<Tokens, ErrMsg> validate_tokens() const noexcept(true);


    // copy invalid
    Tokenizer &operator=(const Tokenizer &rhs);
    explicit Tokenizer(const Tokenizer &other);
};
