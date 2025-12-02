#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 2, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        for (auto pair : split(line, ',')) {
            auto [fs, ls] = split_two(pair, '-');
            auto f = uparse_int(fs);
            auto l = uparse_int(ls);
            for (auto x : range(f, l + 1)) {
                auto ss = to_ts(to_string(x));
                auto p1 = ss.substr(0, ss.size_bytes() / 2);
                auto p2 = ss.substr(ss.size_bytes() / 2);
                if (p1 == p2) {
                    s += x;
                }
            }
        }
    }
    return s;
}

AOC_SOLUTION(2025, 2, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        for (auto pair : split(line, ',')) {
            auto [fs, ls] = split_two(pair, '-');
            auto f = uparse_int(fs);
            auto l = uparse_int(ls);
            for (auto x : range(f, l + 1)) {
                auto ss = to_ts(to_string(x));
                for (auto l : range(1u, ss.size() / 2 + 1)) {
                    auto b = true;
                    for (auto i : range(l, ss.size()) | stride(l)) {
                        if (ss.substr(0, l) != ss.substr(i, l)) {
                            b = false;
                            break;
                        }
                    }
                    if (b) {
                        s += x;
                        goto lll;
                    }
                }
            lll:
            }
        }
    }
    return s;
}
