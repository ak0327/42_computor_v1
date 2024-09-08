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
            std::deque<s_token>::const_iterator *current,
            std::deque<s_token>::const_iterator &end) noexcept(true) {
        return Parser::parse_term(current, end);
    }
};
