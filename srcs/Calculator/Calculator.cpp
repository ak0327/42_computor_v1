#include "Calculator.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include "computor.hpp"

bool DEBUG = false;

Calculator::Calculator(const std::map<int, double> &polynomial)
    : polynomial_(polynomial) {}

Calculator::~Calculator() {}

int Calculator::solve_quadratic_equation() noexcept(true) {
    this->kMinDegree_ = 0;
    this->kMaxDegree_ = 2;

    // D
    QuadraticSolver::SolutionType solution_type = Calculator::solve();
    Calculator::display_solution_type(solution_type);

    // solve
    std::vector<QuadraticSolver::Solution> solutions = this->solutions_;
    Calculator::display_solutions(solutions, solution_type);
    return Calculator::solve_result();
}

// aX^2 + bX^1 + cX^0 = 0
//  a == 0 -> 一次
QuadraticSolver::SolutionType Calculator::solve() noexcept(true) {
    int polynomial_min_degree = this->polynomial_.begin()->first;
    int polynomial_max_degree = this->polynomial_.rbegin()->first;

    if (polynomial_min_degree < this->kMinDegree_) {
        return QuadraticSolver::NoSolutionDegreeTooLow;
    }
    if (this->kMaxDegree_ < polynomial_max_degree) {
        return QuadraticSolver::NoSolutionDegreeTooHigh;
    }

    double a, b, c;
    a = (this->polynomial_.find(2) != this->polynomial_.end()) ? this->polynomial_.at(2) : 0.0;
    b = (this->polynomial_.find(1) != this->polynomial_.end()) ? this->polynomial_.at(1) : 0.0;
    c = (this->polynomial_.find(0) != this->polynomial_.end()) ? this->polynomial_.at(0) : 0.0;

    QuadraticSolver::EquationType eq_type = Calculator::get_equation_type(a, b);
    QuadraticSolver::SolutionType solution_type;
    std::vector<QuadraticSolver::Solution> solutions;
    switch (eq_type) {
        case QuadraticSolver::Quadratic: {
            double D = b * b - 4 * a * c;
            solution_type = Calculator::get_quadratic_eq_solution_type(D);
            this->solutions_ = Calculator::solve_quadratic(a, b, D, solution_type);
            break;
        }
        case QuadraticSolver::Linear:
            solution_type = QuadraticSolver::OneRealSolutionLinear;
            this->solutions_ = Calculator::solve_linear(b, c, solution_type);
            break;
        case QuadraticSolver::Constant:
            solution_type = Calculator::get_constant_eq_solution_type(c);
            break;
    }
    return solution_type;
}

std::vector<QuadraticSolver::Solution> Calculator::solve_quadratic(
        double a,
        double b,
        double D,
        QuadraticSolver::SolutionType type) noexcept(true) {
    std::vector<QuadraticSolver::Solution> solutions;

    switch (type) {
        case QuadraticSolver::TwoComplexSolutionsQuadratic: {
            if (DEBUG) std::cout << "solve_quadratic() 2-complex" << std::endl;
            QuadraticSolver::Solution ans1, ans2;
            ans1 = {
                    .re = Computor::normalize_zero(-b / 2.0 / a),
                    .im = Computor::normalize_zero(std::sqrt(-D) / 2.0 / a)
            };
            ans2 = {
                    .re = Computor::normalize_zero(-b / 2.0 / a),
                    .im = Computor::normalize_zero(-std::sqrt(-D) / 2.0 / a)
            };
            solutions.push_back(ans1);
            solutions.push_back(ans2);
            break;
        }
        case QuadraticSolver::TwoRealSolutionsQuadratic: {
            if (DEBUG) std::cout << "solve_quadratic() 2-real" << std::endl;
            QuadraticSolver::Solution ans1, ans2;
            ans1.re = Computor::normalize_zero((-b + std::sqrt(D)) / 2.0 / a);
            ans2.re = Computor::normalize_zero((-b - std::sqrt(D)) / 2.0 / a);
            solutions.push_back(ans1);
            solutions.push_back(ans2);
            break;
        }
        case QuadraticSolver::OneRealSolutionQuadratic: {
            if (DEBUG) std::cout << "solve_quadratic() 1-real" << std::endl;
            QuadraticSolver::Solution ans;
            ans.re = Computor::normalize_zero(-b / 2.0 / a);
            solutions.push_back(ans);
            break;
        }
        default:
            break;
    }
    return solutions;
}

std::vector<QuadraticSolver::Solution> Calculator::solve_linear(
        double b,
        double c,
        QuadraticSolver::SolutionType type) noexcept(true) {
    std::vector<QuadraticSolver::Solution> solutions;

    switch (type) {
        case QuadraticSolver::OneRealSolutionLinear: {
            if (DEBUG) std::cout << "solve_linear() 1-real" << std::endl;
            QuadraticSolver::Solution ans;
            ans.re = Computor::normalize_zero(-c / b);
            solutions.push_back(ans);
            break;
        }
        default:
            break;
    }
    return solutions;
}

QuadraticSolver::EquationType Calculator::get_equation_type(double a, double b) noexcept(true) {
    // 2次方程式: aX^2 + bX + c = 0
    if (a != 0.0) { return QuadraticSolver::Quadratic; }
    // 1次方程式: bX + c = 0
    if (b != 0.0) { return QuadraticSolver::Linear; }
    // 0次方程式: c = 0
    return QuadraticSolver::Constant;
}

QuadraticSolver::SolutionType Calculator::get_quadratic_eq_solution_type(double D) noexcept(true) {
    if (std::isnan(D) || std::isinf(D)) { return QuadraticSolver::NoSolution; }
    if (D < 0.0) { return QuadraticSolver::TwoComplexSolutionsQuadratic; }
    if (0.0 < D) { return QuadraticSolver::TwoRealSolutionsQuadratic; }
    return QuadraticSolver::OneRealSolutionQuadratic;
}

QuadraticSolver::SolutionType Calculator::get_constant_eq_solution_type(double c) noexcept(true) {
    // c = 0 (解なし) or 0 = 0 (不定形)
    return c != 0.0 ? QuadraticSolver::NoSolution : QuadraticSolver::Indeterminate;
}

void Calculator::display_solution_type(QuadraticSolver::SolutionType type) noexcept(true) {
    std::string solution;
    if (DEBUG) std::cout << get_solution_type(type) << std::endl;

    switch (type) {
        case QuadraticSolver::TwoRealSolutionsQuadratic:
            solution = "Discriminant is positive, the two solutions are:";
            break;
        case QuadraticSolver::TwoComplexSolutionsQuadratic:
            solution = "Discriminant is negative, the two solutions are:";
            break;
        case QuadraticSolver::OneRealSolutionQuadratic:
            solution = "Discriminant is zero, the solution is:";
            break;
        case QuadraticSolver::OneRealSolutionLinear:
            solution = "The solution is:";
            break;
        case QuadraticSolver::Indeterminate:
            solution = "The equation is indeterminate, infinite solutions.";
            break;
        case QuadraticSolver::NoSolutionDegreeTooLow:
            solution = "The polynomial degree is strictly less than 0, I can't solve.";
            break;
        case QuadraticSolver::NoSolutionDegreeTooHigh:
            solution = "The polynomial degree is strictly greater than 2, I can't solve.";
            break;
        case QuadraticSolver::NoSolution:
            solution = "I can't solve.";
            break;
    }
    std::cout << solution << std::endl;
}

void Calculator::display_solutions(
        const std::vector<QuadraticSolver::Solution> &solutions,
        QuadraticSolver::SolutionType type) noexcept(true) {
    for (const auto &solution : solutions) {
        if (0 < solution.re) {
            std::cout << " ";
        }
        std::cout << std::fixed << std::setprecision(2) << solution.re;
        if (type == QuadraticSolver::TwoComplexSolutionsQuadratic) {
            if (0 < solution.im) {
                std::cout << "+";
            }
            std::cout << std::fixed << std::setprecision(2) << solution.im << "i";
        }
        std::cout << std::endl;
    }
}

int Calculator::solve_result() noexcept(true) {
    if (0 < this->solutions_.size()) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

std::string get_solution_type(QuadraticSolver::SolutionType type) noexcept(true) {
    switch (type) {
        case QuadraticSolver::TwoRealSolutionsQuadratic:
            return "Quadratic: 2-real";
        case QuadraticSolver::TwoComplexSolutionsQuadratic:
            return "Quadratic: 2-complex";
        case QuadraticSolver::OneRealSolutionQuadratic:
            return "Quadratic: 1-real";
        case QuadraticSolver::OneRealSolutionLinear:
            return "Linear: 1-real";
        case QuadraticSolver::Indeterminate:
            return "Constant: Indeterminate";
        case QuadraticSolver::NoSolutionDegreeTooLow:
            return "NoSolution: degree too low";
        case QuadraticSolver::NoSolutionDegreeTooHigh:
            return "NoSolution: degree too high";
        case QuadraticSolver::NoSolution:
            return "NoSolution";
        default:
            return "Error";
    }
}
