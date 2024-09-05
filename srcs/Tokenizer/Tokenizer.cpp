#include "Tokenizer.hpp"
#include "Parser.hpp"

Tokenizer::Tokenizer() {}

Tokenizer::~Tokenizer() {}

std::vector<s_token> Tokenizer::tokens() noexcept(true) {
    return this->tokens_;
}

/*
 equation    = 1*expression "=" 1*expression
 expression  = 1*[ *(SP) term *(SP) ]
 term        = [ *(SP) operator *(SP) ] [ coefficient ( *(SP) "*" *(SP) ) ] ALPHA "^" 1*( DIGIT )
 operator    = "+" / "-"
 coefficient = integer / decimal
 decimal     = 1*DIGIT "." 1*DIGIT
 integer     = 1*DIGIT
 ALPHA       = A-Z / a-z
 DIGIT       = 0-9
 SP          = " "
 */
Status Tokenizer::tokenize(const std::string &equation) noexcept(true) {
    if (equation.empty()) {
        return Status::FAILURE;
    }
    std::deque<std::string> split = Tokenizer::split_equation(equation);

    tagging(split);
    return validate_tokens();
}

Status Tokenizer::tagging(const std::deque<std::string> &split) noexcept(true) {
    (void)split;
    // todo
    return Status::SUCCESS;
}

Status Tokenizer::validate_tokens() const noexcept(true) {
    // todo
    return Status::SUCCESS;
}

std::deque<std::string> Tokenizer::split_equation(const std::string &equation) noexcept(true) {
    std::deque<std::string> split;

    // equation
    split = Tokenizer::split_by_delimiter(equation, SP);

    // expression
    bool keep = true;
    split = Tokenizer::split_by_delimiter(split, OP_EQUAL, keep);

    // term
    split = Tokenizer::split_by_delimiter(split, OP_MUL, keep);
    split = Tokenizer::split_by_delimiter(split, OP_PLUS, keep);
    split = Tokenizer::split_by_delimiter(split, OP_MINUS, keep);
    split = Tokenizer::split_by_delimiter(split, OP_POW, keep);
    return split;
}

std::deque<std::string> Tokenizer::split_by_delimiter(
        const std::string &src,
        char delimiter,
        bool keep_delimiter) noexcept(true) {
    std::deque<std::string> split = {};
    std::string token = "";

    for (char c : src) {
        if (c != delimiter) {
            token += c;
        } else {
            if (!token.empty()) {
                split.push_back(token);
                token.clear();
            }
            if (keep_delimiter) {
                split.push_back(std::string(1, c));
            }
        }
    }

    if (!token.empty()) {
        split.push_back(token);
    }
    return split;
}

std::deque<std::string> Tokenizer::split_by_delimiter(
        const std::deque<std::string> &src,
        char delimiter,
        bool keep_delimiter) noexcept(true) {
    std::deque<std::string> split, split_elem;
    std::deque<std::string>::const_iterator itr;

    for (itr = src.begin(); itr != src.end(); ++itr) {
        split_elem = split_by_delimiter(*itr, delimiter, keep_delimiter);
        split.insert(split.end(), split_elem.begin(), split_elem.end());
    }

    return split;
}
