#pragma once

#include "computor.hpp"
#include <string>
#include <map>

class Parser {
 public:
    Parser();
    ~Parser();

    Status parse_equation(const std::string &equation) noexcept(true);
    Status validate_equation(int degree = 3) const noexcept(true);

    std::map<int, int> get_polynomial() const noexcept(true);

 private:
    std::map<int, int> polynomial_;

};
