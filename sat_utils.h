#ifndef SAT_UTILS_H
#define SAT_UTILS_H

#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#ifdef _WIN32
#  include <windows.h>
#  include <psapi.h>
#else
#  include <sys/resource.h>
#endif

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
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return static_cast<long>(pmc.WorkingSetSize / 1024);
    }
    return 0;
#elif defined(__unix__) || defined(__APPLE__)
    struct rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
#  ifdef __APPLE__
        return static_cast<long>(usage.ru_maxrss / 1024);
#  else
        return static_cast<long>(usage.ru_maxrss);
#  endif
    }
    return 0;
#else
    return 0; // Unknown platform
#endif
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
