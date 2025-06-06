#ifndef SAT_UTILS_H
#define SAT_UTILS_H

#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <string>


using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

// Global random number generator used across solvers
inline std::mt19937 rng{std::random_device{}()};

inline void seed_rng(unsigned int seed) { rng.seed(seed); }

// Generate a random 3-SAT formula with given number of variables and clauses
inline CNF generate_random_formula(int variables, int clauses) {
    std::uniform_int_distribution<int> var_dist(1, variables);
    std::uniform_int_distribution<int> sign_dist(0, 1);
    CNF formula;
    for (int i = 0; i < clauses; ++i) {
        Clause clause;
        for (int j = 0; j < 3; ++j) {
            int var = var_dist(rng);
            int lit = sign_dist(rng) ? var : -var;
            clause.push_back(lit);
        }
        formula.push_back(clause);
    }
    return formula;
}

// Generate multiple random formulas using the provided seed
inline std::vector<CNF> generate_random_formulas(int variables, int clauses,
                                                 int instances,
                                                 unsigned int seed) {
    seed_rng(seed);
    std::vector<CNF> formulas;
    for (int i = 0; i < instances; ++i)
        formulas.push_back(generate_random_formula(variables, clauses));
    return formulas;
}

inline long get_memory_usage_kb() {
    return 0;
}

inline void print_result(const std::string& name, int instances,
                        const std::chrono::steady_clock::time_point& start,
                        const std::chrono::steady_clock::time_point& end) {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    long mem_kb = get_memory_usage_kb();
    std::cout << name << " solved " << instances << " instance(s) in "
              << duration << " ms. Memory used: " << mem_kb << " KB\n";
}

#endif // SAT_UTILS_H
