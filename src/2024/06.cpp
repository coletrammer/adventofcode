#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/cartesian_product.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/not_fn.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 6, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto r = 0;
    auto c = 0;
    auto dr = 0;
    auto dc = 0;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch != '#' && ch != '.') {
                r = row;
                c = col;
                switch (ch) {
                    case '^':
                        dr = -1;
                        break;
                    case 'v':
                        dr = 1;
                        break;
                    case '>':
                        dc = 1;
                        break;
                    case '<':
                        dc = -1;
                        break;
                }
                break;
            }
        }
    }

    auto set = Set<Tuple<int, int>> {};
    while (1) {
        set.insert({ r, c });

        auto orr = r;
        auto oc = c;
        r += dr;
        c += dc;
        if (!(r >= 0 && r < (int) lines.size() && c >= 0 && c < (int) lines[0].size())) {
            break;
        }

        if (lines[r][c] != '#') {
            continue;
        }

        r = orr;
        c = oc;

        auto drr = dr;
        dr = dc;
        dc = -drr;
    }
    return set.size();
}

AOC_SOLUTION(2024, 6, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    auto rrr = 0;
    auto ccc = 0;
    auto drrr = 0;
    auto dccc = 0;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch != '#' && ch != '.') {
                rrr = row;
                ccc = col;
                switch (ch) {
                    case '^':
                        drrr = -1;
                        break;
                    case 'v':
                        drrr = 1;
                        break;
                    case '>':
                        dccc = 1;
                        break;
                    case '<':
                        dccc = -1;
                        break;
                }
                break;
            }
        }
    }

    for (auto [sr, sc] : cartesian_product(range(lines.size()), range(lines[0].size()))) {
        if ((int) sr == rrr && (int) sc == ccc) {
            continue;
        }
        if (lines[sr][sc] != '.') {
            continue;
        }
        lines[sr][sc] = '#';

        auto r = rrr;
        auto c = ccc;
        auto dr = drrr;
        auto dc = dccc;
        auto set = Set<Tuple<int, int, int, int>> {};
        while (1) {
            if (set.contains({ r, c, dr, dc })) {
                s++;
                break;
            }
            set.insert({ r, c, dr, dc });

            auto orr = r;
            auto oc = c;
            r += dr;
            c += dc;
            if (!(r >= 0 && r < (int) lines.size() && c >= 0 && c < (int) lines[0].size())) {
                break;
            }

            if (lines[r][c] != '#') {
                continue;
            }

            r = orr;
            c = oc;

            auto drr = dr;
            dr = dc;
            dc = -drr;
        }
        lines[sr][sc] = '.';
    }
    return s;
}
