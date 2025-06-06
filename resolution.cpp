#include "sat_utils.h"
#include <set>

bool resolution_unsat(CNF formula) {
    for (auto &c : formula) std::sort(c.begin(), c.end());
    std::set<Clause> clauses(formula.begin(), formula.end());
    bool changed = true;
    while (changed && clauses.size() < 500) {
        changed = false;
        std::vector<Clause> new_clauses;
        for (auto it1 = clauses.begin(); it1 != clauses.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != clauses.end(); ++it2) {
                for (int lit : *it1) {
                    if (std::find(it2->begin(), it2->end(), -lit) != it2->end()) {
                        Clause resolvent;
                        for (int l : *it1) if (l != lit) resolvent.push_back(l);
                        for (int l : *it2) if (l != -lit && std::find(resolvent.begin(), resolvent.end(), l) == resolvent.end()) resolvent.push_back(l);
                        std::sort(resolvent.begin(), resolvent.end());
                        if (resolvent.empty()) return true; // empty clause
                        if (!clauses.count(resolvent)) {
                            new_clauses.push_back(resolvent);
                        }
                    }
                }
            }
        }
        if (!new_clauses.empty()) {
            changed = true;
            for (auto &c : new_clauses) clauses.insert(c);
        }
    }
    return false; // satisfiable if no empty clause derived
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
        resolution_unsat(f);
    }
    auto end = std::chrono::steady_clock::now();
    print_result("Resolution", instances, start, end);
    return 0;
}
