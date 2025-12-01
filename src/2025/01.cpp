#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 1, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto c = 50_i32;
    for (auto [row, line] : enumerate(lines)) {
        auto dir = line[0] == 'L';
        auto n = *parse_int(line.substr(1));
        if (dir) {
            c -= n;
        } else {
            c += n;
        }

        c += 100;
        c %= 100;
        if (c == 0) {
            s++;
        }
    }
    return s;
}

AOC_SOLUTION(2025, 1, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto c = 50_i32;
    for (auto [row, line] : enumerate(lines)) {
        auto dir = line[0] == 'L';
        auto n = *parse_int(line.substr(1));
        for (auto _ : range(n)) {
            if (dir) {
                c--;
            } else {
                c++;
            }

            c += 100;
            c %= 100;
            if (c == 0) {
                s++;
            }
        }
    }
    return s;
}
