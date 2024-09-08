#pragma once
#include "Parser.hpp"
#include "gtest/gtest.h"

class TestParser : public ::testing::Test {
 public:
    static void skip_sp(
            const std::string &str,
            std::size_t start_pos,
            std::size_t *end_pos) noexcept(true) {
        return Parser::skip_sp(str, start_pos, end_pos);
    }

    static s_term parse_term(
            const std::deque<s_token> &tokens,
            std::deque<s_token>::const_iterator *itr) noexcept(true) {
        return Parser::parse_term(tokens, itr);
    }
};
