#include "Calculator.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>

bool DEBUG = true;

Calculator::Calculator(const std::map<int, double> &polynomial)
    : polynomial_(polynomial) {}

Calculator::~Calculator() {}

void Calculator::solve_quadratic_equation() noexcept(true) {
    this->kMinDegree_ = 0;
    this->kMaxDegree_ = 2;

    // D
    SolutionType solution_type = Calculator::solve();
    Calculator::display_solution_type(solution_type);

    // solve
    std::vector<Solution> solutions = this->solutions_;
    Calculator::display_solutions(solutions, solution_type);
}

// aX^2 + bX^1 + cX^0 = 0
//  a == 0 -> 一次
SolutionType Calculator::solve() noexcept(true) {
    int polynomial_min_degree = polynomial_.begin()->first;
    int polynomial_max_degree = polynomial_.rbegin()->first;

    if (polynomial_min_degree < this->kMinDegree_) {
        return NoSolutionDegreeTooLow;
    }
    if (this->kMaxDegree_ < polynomial_max_degree) {
        return NoSolutionDegreeTooHigh;
    }

    try {
        double a, b, c;
        a = this->polynomial_.at(2);
        b = this->polynomial_.at(1);
        c = this->polynomial_.at(0);

        EquationType eq_type = Calculator::get_equation_type(a, b);
        SolutionType solution_type;
        std::vector<Solution> solutions;
        switch (eq_type) {
            case Quadratic: {
                double D = b * b - 4 * a * c;
                // std::cout << "D:" << D << std::endl;
                solution_type = Calculator::get_quadratic_eq_solution_type(D);
                this->solutions_ = Calculator::solve_quadratic(a, b, D, solution_type);
                break;
            }
            case Linear:
                solution_type = OneRealSolutionLinear;
                this->solutions_ = Calculator::solve_linear(b, c, solution_type);
                break;
            case Constant:
                solution_type = Calculator::get_constant_eq_solution_type(c);
                break;
        }
        return solution_type;
    } catch (const std::exception &e) {
        // a, b, c not found
        return NoSolution;
    }
}

std::vector<Solution> Calculator::solve_quadratic(
        double a,
        double b,
        double D,
        SolutionType type) noexcept(true) {
    std::vector<Solution> solutions;

    switch (type) {
        case TwoComplexSolutionsQuadratic: {
            if (DEBUG) std::cout << "solve_quadratic() 2-complex" << std::endl;
            Solution ans1, ans2;
            ans1 = {
                    .re = -b / 2.0 / a,
                    .im = std::sqrt(-D) / 2.0 / a
            };
            ans2 = {
                    .re = -b / 2.0 / a,
                    .im = -std::sqrt(-D) / 2.0 / a
            };
            solutions.push_back(ans1);
            solutions.push_back(ans2);
            break;
        }
        case TwoRealSolutionsQuadratic: {
            if (DEBUG) std::cout << "solve_quadratic() 2-real" << std::endl;
            Solution ans1, ans2;
            ans1.re = (-b + std::sqrt(D)) / 2.0 / a;
            ans2.re = (-b - std::sqrt(D)) / 2.0 / a;
            solutions.push_back(ans1);
            solutions.push_back(ans2);
            break;
        }
        case OneRealSolutionQuadratic: {
            if (DEBUG) std::cout << "solve_quadratic() 1-real" << std::endl;
            Solution ans;
            ans.re = -b / 2.0 / a;
            solutions.push_back(ans);
            break;
        }
        default:
            break;
    }
    return solutions;
}

std::vector<Solution> Calculator::solve_linear(
        double b,
        double c,
        SolutionType type) noexcept(true) {
    std::vector<Solution> solutions;

    switch (type) {
        case OneRealSolutionLinear: {
            if (DEBUG) std::cout << "solve_linear() 1-real" << std::endl;
            Solution ans;
            ans.re = -c / b;
            solutions.push_back(ans);
            break;
        }
        default:
            break;
    }
    return solutions;
}

EquationType Calculator::get_equation_type(double a, double b) noexcept(true) {
    // 2次方程式: aX^2 + bX + c = 0
    if (a != 0.0) { return Quadratic; }
    // 1次方程式: bX + c = 0
    if (b != 0.0) { return Linear; }
    // 0次方程式: c = 0
    return Constant;
}

SolutionType Calculator::get_quadratic_eq_solution_type(double D) noexcept(true) {
    if (std::isnan(D) || std::isinf(D)) { return NoSolution; }
    if (D < 0.0) { return TwoComplexSolutionsQuadratic; }
    if (0.0 < D) { return TwoRealSolutionsQuadratic; }
    return OneRealSolutionQuadratic;
}

SolutionType Calculator::get_constant_eq_solution_type(double c) noexcept(true) {
    // c = 0 (解なし) or 0 = 0 (不定形)
    return c != 0.0 ? NoSolution : Indeterminate;
}

void Calculator::display_solution_type(SolutionType type) noexcept(true) {
    std::string solution;
    if (DEBUG) std::cout << get_solution_type(type) << std::endl;

    switch (type) {
        case TwoRealSolutionsQuadratic:
            solution = "Discriminant is positive, the two solutions are:";
            break;
        case TwoComplexSolutionsQuadratic:
            solution = "Discriminant is negative, the two solutions are:";
            break;
        case OneRealSolutionQuadratic:
            solution = "Discriminant is zero, the solution is:";
            break;
        case OneRealSolutionLinear:
            solution = "The solution is:";
            break;
        case Indeterminate:
            solution = "The equation is indeterminate, infinite solutions.";
            break;
        case NoSolutionDegreeTooLow:
            solution = "The polynomial degree is strictly less than 0, I can't solve.";
            break;
        case NoSolutionDegreeTooHigh:
            solution = "The polynomial degree is strictly greater than 2, I can't solve.";
            break;
        case NoSolution:
            solution = "I can't solve.";
            break;
    }
    std::cout << solution << std::endl;
}

void Calculator::display_solutions(
        const std::vector<Solution> &solutions,
        SolutionType type) noexcept(true) {
    for (const auto &solution : solutions) {
        if (0 < solution.re) {
            std::cout << " ";
        }
        std::cout << std::fixed << std::setprecision(2) << solution.re;
        if (type == TwoComplexSolutionsQuadratic) {
            if (0 < solution.im) {
                std::cout << "+";
            }
            std::cout << std::fixed << std::setprecision(2) << solution.im << "i";
        }
        std::cout << std::endl;
    }
}

std::string get_solution_type(SolutionType type) noexcept(true) {
    switch (type) {
        case TwoRealSolutionsQuadratic:
            return "Quadratic: 2-real";
        case TwoComplexSolutionsQuadratic:
            return "Quadratic: 2-complex";
        case OneRealSolutionQuadratic:
            return "Quadratic: 1-real";
        case OneRealSolutionLinear:
            return "Linear: 1-real";
        case Indeterminate:
            return "Constant: Indeterminate";
        case NoSolutionDegreeTooLow:
            return "NoSolution: degree too low";
        case NoSolutionDegreeTooHigh:
            return "NoSolution: degree too high";
        case NoSolution:
            return "NoSolution";
        default:
            return "Error";
    }
}
