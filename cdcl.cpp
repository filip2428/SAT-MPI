#include "sat_utils.h"

CNF assign_literal_cl(const CNF& f, int lit, bool& conflict) {
    CNF out;
    for (const auto& c : f) {
        if (std::find(c.begin(), c.end(), lit) != c.end()) continue;
        Clause new_c;
        for (int l : c) if (l != -lit) new_c.push_back(l);
        if (new_c.empty()) {
            conflict = true;
            return {};
        }
        out.push_back(new_c);
    }
    return out;
}

bool cdcl_recursive(CNF formula) {
    while (true) {
        bool unit = false;
        for (const auto& c : formula) {
            if (c.size() == 1) {
                bool conflict = false;
                formula = assign_literal_cl(formula, c[0], conflict);
                if (conflict) return false;
                unit = true;
                break;
            }
        }
        if (!unit) break;
    }
    if (formula.empty()) return true;
    for (const auto& c : formula) if (c.empty()) return false;
    int lit = formula[0][0];
    bool conflict = false;
    CNF f_true = assign_literal_cl(formula, lit, conflict);
    if (!conflict && cdcl_recursive(f_true)) return true;
    // learn clause { -lit }
    formula.push_back({-lit});
    conflict = false;
    CNF f_false = assign_literal_cl(formula, -lit, conflict);
    if (!conflict) return cdcl_recursive(f_false);
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
        cdcl_recursive(f);
    }
    auto end = std::chrono::steady_clock::now();
    print_result("CDCL", instances, start, end);
    return 0;
}
