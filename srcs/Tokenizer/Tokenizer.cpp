#include "Tokenizer.hpp"
#include <iostream>
#include <sstream>

Tokenizer::Tokenizer() {}

Tokenizer::~Tokenizer() {}

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
Result<Tokens, ErrMsg> Tokenizer::tokenize(const std::string &equation) noexcept(true) {
    if (equation.empty()) {
        return Result<Tokens, ErrMsg>::err("invalid equation");
    }
    std::deque<std::string> split = Tokenizer::split_equation(equation);

    tagging(split);

    this->tokens_ = split_term_coef_and_base(this->tokens_);
    tagging_terms();
    return validate_tokens();
}

const std::deque<s_token> &Tokenizer::tokens() noexcept(true) {
    return this->tokens_;
}


////////////////////////////////////////////////////////////////////////////////


std::deque<std::string> Tokenizer::split_equation(
        const std::string &equation) noexcept(true) {
    std::deque<std::string> split;

    // equation
    split = Tokenizer::split_by_delimiter(equation, Computor::SP);

    // expression
    bool keep_delimiter = true;
    split = Tokenizer::split_by_delimiter(split, Computor::OP_EQUAL, keep_delimiter);

    // term
    split = Tokenizer::split_by_delimiter(split, Computor::OP_MUL, keep_delimiter);
    split = Tokenizer::split_by_delimiter(split, Computor::OP_PLUS, keep_delimiter);
    split = Tokenizer::split_by_delimiter(split, Computor::OP_MINUS, keep_delimiter);
    split = Tokenizer::split_by_delimiter(split, Computor::OP_POW, keep_delimiter);
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

    for (const auto &itr : src) {
        split_elem = split_by_delimiter(itr, delimiter, keep_delimiter);
        split.insert(split.end(), split_elem.begin(), split_elem.end());
    }

    return split;
}

// kind none -> split [coef][base], like 2X -> [2][X]
std::deque<s_token> Tokenizer::split_term_coef_and_base(
        const std::deque<s_token> &tokens) noexcept(true) {
    std::deque<s_token> split, new_tokens;

    for (auto &token : tokens) {
        new_tokens = {};

        std::string word;
        if (token.kind == None && std::isdigit(token.word[0])) {
            std::size_t pos = 0;
            while (token.word[pos] && !std::isalpha(token.word[pos])) {
                ++pos;
            }
            if (pos < token.word.length()) {
                std::string coef = token.word.substr(0, pos);
                std::string base = token.word.substr(pos);
                s_token coef_token = {}; coef_token.word = coef;
                s_token base_token = {}; base_token.word = base;
                new_tokens.push_back(coef_token);
                new_tokens.push_back(base_token);
            }
        }

        if (new_tokens.empty()) {
            new_tokens.push_back(token);
        }

        split.insert(split.end(), new_tokens.begin(), new_tokens.end());
    }

    return split;
}


////////////////////////////////////////////////////////////////////////////////


Computor::Status Tokenizer::tagging(const std::deque<std::string> &split) noexcept(true) {
    Tokenizer::init_tokens(split);
    Tokenizer::tagging_operators();
    Tokenizer::tagging_terms();
    return Computor::Status::SUCCESS;
}

void Tokenizer::init_tokens(
        const std::deque<std::string> &split) noexcept(true) {
    this->tokens_ = {};

    for (auto itr = split.cbegin(); itr != split.cend(); ++itr) {
        s_token token = {};
        token.word = *itr;
        token.kind = None;
        this->tokens_.push_back(token);
    }
}

void Tokenizer::tagging_operators() noexcept(true) {
    // +, -, =, *, ^
    for (auto &token : this->tokens_) {
        if (token.word == std::string(1, Computor::OP_PLUS)) {
            token.kind = OperatorPlus;
        } else if (token.word == std::string(1, Computor::OP_MINUS)) {
            token.kind = OperatorMinus;
        } else if (token.word == std::string(1, Computor::OP_MUL)) {
            token.kind = OperatorMul;
        } else if (token.word == std::string(1, Computor::OP_EQUAL)) {
            token.kind = OperatorEqual;
        } else if (token.word == std::string(1, Computor::OP_POW)) {
            token.kind = TermPowSymbol;
        }
    }
}

void Tokenizer::tagging_terms() noexcept(true) {
    // aX^b
    // ^^ ^ tagging
    TokenKind prev, next;
    prev = None;
    next = None;
    for (auto itr = this->tokens_.begin(); itr != this->tokens_.end(); ++itr) {
        auto &token = *itr;
        auto next_it = std::next(itr);
        if (next_it != this->tokens_.end()) {
            auto &next_token = *next_it;
            next = next_token.kind;
        }

        if (token.kind == None) {
            if (Tokenizer::is_term_base(token.word)) {
                token.kind = TermBase;
            } else if (Tokenizer::is_term_coef(token.word, prev, next)) {
                token.kind = TermCoef;
            } else if (Tokenizer::is_term_pow(token.word, prev, next)) {
                token.kind = TermPower;
            }
        }
        prev = token.kind;
        next = None;
    }
}

bool Tokenizer::is_term_base(const std::string &str) noexcept(true) {
    return (str.length() == 1 && std::isalpha(str[0]));
}

bool Tokenizer::is_term_coef(
        const std::string &str,
        TokenKind prev_kind,
        TokenKind next_kind) noexcept(true) {
    if (!Tokenizer::is_num(str)) {
        return false;
    }
    if (prev_kind == TermPowSymbol || prev_kind == OperatorMul) {
        return false;
    }
    if (next_kind == TermPowSymbol) {
        return false;
    }
    return true;
}

bool Tokenizer::is_term_pow(
        const std::string &str,
        TokenKind prev_kind,
        TokenKind next_kind) noexcept(true) {
    if (!Tokenizer::is_num(str)) {
        return false;
    }
    if (prev_kind != TermPowSymbol) {
        return false;
    }
    return (next_kind == None
            || next_kind == OperatorPlus
            || next_kind == OperatorMinus
            || next_kind == OperatorEqual);
}

bool Tokenizer::is_num(const std::string &str) noexcept(true) {
    if (str.empty() || !std::isdigit(str[0])) {
        // NG: .1
        return false;
    }

    if (str.find('E') != std::string::npos
        || str.find('e') != std::string::npos) {
        // NG: E+10
        return false;
    }

    std::size_t end;
    try {
        std::stod(str, &end);
        return end == str.length();
    } catch (const std::invalid_argument &) {
        return false;
    } catch (const std::out_of_range &) {
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////


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
Result<Tokens, ErrMsg> Tokenizer::validate_tokens() const noexcept(true) {
    std::string prev_word;

    for (auto &token : this->tokens_) {
        if (token.kind == None) {
            std::ostringstream err_oss;
            err_oss << "unexpected token [" << token.word << "]";
            if (!prev_word.empty()) {
                err_oss << ", near " << prev_word;
            }
            return Result<Tokens, ErrMsg>::err(err_oss.str());
        }
        prev_word = token.word;
    }
    return Result<Tokens, ErrMsg>::ok(this->tokens_);
}
