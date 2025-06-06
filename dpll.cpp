#include "sat_utils.h"

CNF assign_literal(const CNF& f, int lit, bool& conflict) {
    CNF out;
    for (const auto& c : f) {
        if (std::find(c.begin(), c.end(), lit) != c.end()) continue; // clause satisfied
        Clause new_c;
        bool empty = true;
        for (int l : c) {
            if (l == -lit) continue; // remove
            new_c.push_back(l);
        }
        if (new_c.empty()) {
            conflict = true;
            return {};
        }
        out.push_back(new_c);
    }
    return out;
}

bool dpll(CNF f) {
    while (true) {
        bool unit = false;
        for (const auto& c : f) {
            if (c.size() == 1) {
                bool conflict = false;
                f = assign_literal(f, c[0], conflict);
                if (conflict) return false;
                unit = true;
                break;
            }
        }
        if (!unit) break;
    }
    if (f.empty()) return true;
    for (const auto& c : f) if (c.empty()) return false;
    int lit = f[0][0];
    bool conflict = false;
    CNF f_true = assign_literal(f, lit, conflict);
    if (!conflict && dpll(f_true)) return true;
    conflict = false;
    CNF f_false = assign_literal(f, -lit, conflict);
    if (!conflict) return dpll(f_false);
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
        dpll(f);
    }
    auto end = std::chrono::steady_clock::now();
    print_result("DPLL", instances, start, end);
    return 0;
}
