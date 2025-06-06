#ifndef SAT_UTILS_H
#define SAT_UTILS_H

#include <vector>
#include <random>
#include <chrono>


using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

// Generate a random 3-SAT formula with given number of variables and clauses
inline CNF generate_random_formula(int variables, int clauses) {
    static std::mt19937 rng(std::random_device{}());
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

// Return memory used by the current process in kilobytes.
inline long get_memory_usage_kb() {
    PROCESS_MEMORY_COUNTERS pmc{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return static_cast<long>(pmc.WorkingSetSize / 1024);
#elif defined(__unix__) || defined(__APPLE__)
        return static_cast<long>(usage.ru_maxrss / 1024);
        return static_cast<long>(usage.ru_maxrss);
#else
    return 0; // Unknown platform
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    long mem_kb = get_memory_usage_kb();
    std::cout << name << " solved " << instances << " instance(s) in "
              << duration << " ms. Memory used: " << mem_kb << " KB\n";
}

#endif // SAT_UTILS_H
