#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 7, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto cols = di::TreeSet<usize> {};
    for (auto [row, line] : enumerate(lines)) {
        if (cols.empty()) {
            // Look for start.
            cols.insert(line.find('S').begin() - line.begin());
            continue;
        }

        auto ncs = di::TreeSet<usize> {};
        for (auto c : cols) {
            if (c >= line.size()) {
                continue;
            }
            if (line[c] == '^') {
                ncs.insert(c - 1);
                ncs.insert(c + 1);
                s++;
            } else {
                ncs.insert(c);
            }
        }
        cols = di::move(ncs);
    }
    return s;
}

AOC_SOLUTION(2025, 7, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto cols = di::TreeMap<usize, usize> {};
    for (auto [row, line] : enumerate(lines)) {
        if (cols.empty()) {
            // Look for start.
            cols[usize(line.find('S').begin() - line.begin())] = 1;
            continue;
        }

        auto ncs = di::TreeMap<usize, usize> {};
        for (auto [c, vals] : cols) {
            if (c >= line.size()) {
                continue;
            }
            if (line[c] == '^') {
                ncs[c - 1] += vals;
                ncs[c + 1] += vals;
            } else {
                ncs[c] += vals;
            }
        }
        cols = di::move(ncs);
    }
    s = 0;
    for (auto [_, v] : cols) {
        s += v;
    }
    return s;
}
