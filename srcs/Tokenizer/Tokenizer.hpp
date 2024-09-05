#pragma once

#include "computor.hpp"
#include <string>
#include <deque>

enum TokenKind {
    Coefficient,    // 係数
    Term,           // x^n
    Operator,       // + or -
    Equal,          // =
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
    Status validate_tokens() const noexcept(true);

    static std::deque<std::string> split_equation(const std::string &equation) noexcept(true);

    static std::deque<std::string> split_by_delimiter(
            const std::string &src,
            char delimiter,
            bool keep_delimiter = false) noexcept(true);

    static std::deque<std::string> split_by_delimiter(
            const std::deque<std::string> &src,
            char delimiter,
            bool keep_delimiter = false) noexcept(true);

    std::vector<s_token> tokens() noexcept(true);

 private:
    std::vector<s_token> tokens_;

    // copy invalid
    Tokenizer &operator=(const Tokenizer &rhs);
    explicit Tokenizer(const Tokenizer &other);

};
