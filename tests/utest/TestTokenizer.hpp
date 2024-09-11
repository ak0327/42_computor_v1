#pragma once

# include "Tokenizer.hpp"
# include "gtest/gtest.h"

class TestTokenizer : public ::testing::Test {
 public:
    static std::deque<std::string> split_by_delimiter(
            const std::string &src,
            char delimiter,
            bool keep_delimiter = false) noexcept(true) {
        return Tokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    }

    static std::deque<std::string> split_by_delimiter(
            const std::deque<std::string> &src,
            char delimiter,
            bool keep_delimiter = false) noexcept(true) {
        return Tokenizer::split_by_delimiter(src, delimiter, keep_delimiter);
    }

    static std::deque<std::string> split_equation(
            const std::string &equation) noexcept(true) {
        return Tokenizer::split_equation(equation);
    }

    static std::deque<s_token> split_coef_and_base(
            const std::deque<s_token> &tokens) noexcept(true) {
        return Tokenizer::split_coef_and_base(tokens);
    }

    static bool is_char(const std::string &str) noexcept(true) {
        return Tokenizer::is_char(str);
    }

    static bool is_integer(const std::string &str) noexcept(true) {
        return Tokenizer::is_integer(str);
    }

    static bool is_decimal(const std::string &str) noexcept(true) {
        return Tokenizer::is_decimal(str);
    }

};
