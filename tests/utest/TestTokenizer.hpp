#pragma once
#include "Tokenizer.hpp"
#include "gtest/gtest.h"

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

    static std::deque<s_token> split_term_coef_and_base(
            const std::deque<s_token> &tokens) noexcept(true) {
        return Tokenizer::split_term_coef_and_base(tokens);
    }
};
