#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 4, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch != '@') {
                continue;
            }
            auto x = 0;
            for (auto [_, _, cc] : neighbors(lines, row, col)) {
                if (cc == '@') {
                    x++;
                }
            }
            if (x < 4) {
                s++;
            }
        }
    }
    return s;
}

AOC_SOLUTION(2025, 4, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    while (true) {
        auto os = s;
        for (auto [row, line] : enumerate(lines)) {
            for (auto [col, ch] : enumerate(line)) {
                if (ch != '@') {
                    continue;
                }
                auto x = 0;
                for (auto [_, _, cc] : neighbors(lines, row, col)) {
                    if (cc == '@') {
                        x++;
                    }
                }
                if (x < 4) {
                    s++;
                    lines[row][col] = '.';
                }
            }
        }
        if (os == s) {
            break;
        }
    }
    return s;
}
