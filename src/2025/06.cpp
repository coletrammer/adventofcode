#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 6, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto vv = di::Vector<di::Vector<i64>> {};
    for (auto [row, line] : enumerate(lines)) {
        if (line.starts_with('*') || line.starts_with('+')) {
            auto i = 0;
            for (auto ch : line) {
                if (ch == ' ') {
                    continue;
                }
                if (ch == '+') {
                    auto r = 0_i64;
                    for (auto& v : vv) {
                        r += v[i];
                    }
                    s += r;
                } else {
                    auto r = 1_i64;
                    for (auto& v : vv) {
                        r *= v[i];
                    }
                    s += r;
                }
                i++;
            }
        } else {
            vv.push_back(all_nums(line));
        }
    }
    return s;
}

AOC_SOLUTION(2025, 6, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto c = (i64) (di::max(lines | di::transform(di::size)) - 1);
    auto ns = di::Vector<i64> {};
    while (c >= 0) {
        auto x = 0_i64;
        for (auto r : range(lines.size() - 1)) {
            auto ch = lines[r].at(c).value_or(' ');
            if (ch == ' ') {
                continue;
            }
            x *= 10;
            x += (ch - '0');
        }
        if (x == 0) {
            c--;
            continue;
        }
        ns.push_back(x);
        auto ch = lines.back().value().at(c).value_or(' ');
        if (ch != ' ') {
            if (ch == '+') {
                s += di::sum(ns);
            } else {
                s += di::product(ns);
            }
            ns.clear();
        }
        c--;
    }
    return s;
}
