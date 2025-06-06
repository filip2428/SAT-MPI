#include "sat_utils.h"
#include <algorithm>
#include <iostream>

CNF assign_literal_gr(const CNF& f, int lit, bool& conflict) {
    CNF out;
    for (const auto& c : f) {
        if (std::find(c.begin(), c.end(), lit) != c.end()) continue;
        Clause new_c;
        for (int l : c) if (l != -lit) new_c.push_back(l);
        if (new_c.empty()) { conflict = true; return {}; }
        out.push_back(new_c);
    }
    return out;
}

bool grasp_recursive(CNF formula) {
    while (true) {
        bool unit = false;
        for (const auto& c : formula) {
            if (c.size() == 1) {
                bool conflict = false;
                formula = assign_literal_gr(formula, c[0], conflict);
                if (conflict) return false;
                unit = true;
                break;
            }
        }
        if (!unit) break;
    }
    if (formula.empty()) return true;
    for (const auto& c : formula) if (c.empty()) return false;
    // pick random literal
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, formula.size() - 1);
    int lit = formula[dist(rng)][0];
    bool conflict = false;
    CNF f_true = assign_literal_gr(formula, lit, conflict);
    if (!conflict && grasp_recursive(f_true)) return true;
    // learn clause { -lit }
    formula.push_back({-lit});
    conflict = false;
    CNF f_false = assign_literal_gr(formula, -lit, conflict);
    if (!conflict) return grasp_recursive(f_false);
    return false;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <instances>\n";
        return 1;
    }
    int instances = std::stoi(argv[1]);
    const int vars = 6;
    const int clauses = 10;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < instances; ++i) {
        CNF f = generate_random_formula(vars, clauses);
        grasp_recursive(f);
    }
    auto end = std::chrono::steady_clock::now();
    print_result("GRASP", instances, start, end);
    return 0;
}
