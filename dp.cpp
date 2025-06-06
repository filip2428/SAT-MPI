#include "sat_utils.h"

bool dp_unsat(CNF formula) {
    while (true) {
        if (formula.empty()) return false; // no clauses -> sat
        for (const auto& c : formula) if (c.empty()) return true; // empty clause -> unsat
        int var = 0;
        for (const auto& c : formula) {
            if (!c.empty()) { var = std::abs(c.front()); break; }
        }
        if (var == 0) return false;
        CNF pos, neg, rest;
        for (const auto& c : formula) {
            if (std::find(c.begin(), c.end(), var) != c.end()) pos.push_back(c);
            else if (std::find(c.begin(), c.end(), -var) != c.end()) neg.push_back(c);
            else rest.push_back(c);
        }
        if (pos.empty() || neg.empty()) { // pure literal
            formula = rest;
            continue;
        }
        for (const auto& c1 : pos) {
            for (const auto& c2 : neg) {
                Clause r;
                for (int lit : c1) if (lit != var) if (std::find(r.begin(), r.end(), lit) == r.end()) r.push_back(lit);
                for (int lit : c2) if (lit != -var && std::find(r.begin(), r.end(), lit) == r.end()) r.push_back(lit);
                if (r.empty()) return true;
                rest.push_back(r);
            }
        }
        formula = rest;
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <instances> <seed>\n";
        return 1;
    }
    int instances = std::stoi(argv[1]);
    unsigned int seed = std::stoul(argv[2]);
    const int vars = 6;
    const int clauses = 10;
    auto start = std::chrono::steady_clock::now();
    auto formulas = generate_random_formulas(vars, clauses, instances, seed);
    for (const auto& f : formulas) {
        dp_unsat(f);
    }
    auto end = std::chrono::steady_clock::now();
    print_result("DP", instances, start, end);
    return 0;
}
