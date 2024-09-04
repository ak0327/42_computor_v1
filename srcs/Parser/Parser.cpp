#include "Parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

Status Parser::parse_equation(const std::string &equation) noexcept(true) {
    (void)equation;

    // 左辺、右辺

    // 移項

    return Status::SUCCESS;
}

Status Parser::validate_equation(int degree) const noexcept(true) {
    (void)degree;

    return Status::SUCCESS;
}

std::map<int, int> Parser::get_polynomial() const noexcept(true) {
    return this->polynomial_;
}
