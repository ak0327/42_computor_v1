#pragma once

#include <map>
#include <string>

class Calculator {
 public:
    explicit Calculator(const std::map<int, double>&polynomial);
    ~Calculator();

    void solve_quadratic_equation();

 private:
    int max_degree_ = 2;
    const std::map<int, double> polynomial_;

    Calculator();
    Calculator(const Calculator &other);
    Calculator &operator=(const Calculator &rhs);

    void discriminate();
    void solve();
};
