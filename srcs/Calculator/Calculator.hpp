#pragma once

# include <map>
# include <string>
# include <vector>


namespace QuadraticSolver {

enum EquationType {
    Quadratic,
    Linear,
    Constant,
};

enum SolutionType {
    // 2次方程式
    TwoRealSolutionsQuadratic,      // 異なる2つの実数解
    TwoComplexSolutionsQuadratic,   // 異なる2つの虚数解
    OneRealSolutionQuadratic,       // ただ1つの実数解（重解）

    // 1次方程式
    OneRealSolutionLinear,          // 1次方程式の実数解

    // 不定形 0 = 0
    Indeterminate,                  // 不定形: 任意の解が存在する

    // 解なし
    NoSolution,                     // 方程式に解が存在しない

    // エラー
    NoSolutionDegreeTooHigh,        // 次数が高すぎる場合のエラー
    NoSolutionDegreeTooLow          // 次数が低すぎる場合のエラー
};

struct Solution {
    double re;
    double im;
};

}  // namespace QuadraticSolver


class Calculator {
 public:
    explicit Calculator(const std::map<int, double>&polynomial);
    ~Calculator();

    void solve_equation() noexcept(true);
    int solve_quadratic_equation() noexcept(true);

 private:
    const std::map<int, double> polynomial_;
    int kMinDegree_, kMaxDegree_;

    std::vector<QuadraticSolver::Solution> solutions_;

    QuadraticSolver::SolutionType solve() noexcept(true);
    int solve_result() noexcept(true);
    static QuadraticSolver::EquationType get_equation_type(double a, double b) noexcept(true);
    static QuadraticSolver::SolutionType get_quadratic_eq_solution_type(double D) noexcept(true);
    static QuadraticSolver::SolutionType get_constant_eq_solution_type(double c) noexcept(true);

    static void display_solution_type(QuadraticSolver::SolutionType type) noexcept(true);

    static std::vector<QuadraticSolver::Solution> solve_quadratic(
            double a,
            double b,
            double D,
            QuadraticSolver::SolutionType type) noexcept(true);
    static std::vector<QuadraticSolver::Solution> solve_linear(
            double b,
            double c,
            QuadraticSolver::SolutionType type) noexcept(true);
    static void display_solutions(
            const std::vector<QuadraticSolver::Solution> &solutions,
            QuadraticSolver::SolutionType type) noexcept(true);

    // invalid
    Calculator();
    Calculator(const Calculator &other);
    Calculator &operator=(const Calculator &rhs);
};

std::string get_solution_type(QuadraticSolver::SolutionType type) noexcept(true);
