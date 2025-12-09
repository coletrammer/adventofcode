#include <di/container/view/slide.h>
#include <di/math/midpoint.h>

#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 9, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto coords = di::Vector<Tuple<i64, i64>> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [x, y] = split_two(line.view(), ',');
        coords.emplace_back(uparse_int(x), uparse_int(y));
    }

    for (auto i : range(coords.size() - 1)) {
        for (auto j : range(i + 1, coords.size())) {
            auto [x1, y1] = coords[i];
            auto [x2, y2] = coords[j];
            auto a = (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1);
            s = max(s, a);
        }
    }
    return s;
}

AOC_SOLUTION(2025, 9, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto coords = di::Vector<Tuple<i64, i64>> {};
    auto good = di::TreeMap<Tuple<i64, i64>, bool> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [x, y] = split_two(line.view(), ',');
        coords.emplace_back(uparse_int(x), uparse_int(y));
    }

    coords.push_back(coords[0]);
    for (auto i : range(coords.size() - 1)) {
        auto j = i + 1;
        auto [x1, y1] = coords[i];
        auto [x2, y2] = coords[j];

        auto [xmin, xmax] = minmax(x1, x2);
        auto [ymin, ymax] = minmax(y1, y2);
        for (auto x : range(xmin, xmax + 1)) {
            good[{ x, ymin }] = true;
            good[{ x, ymax }] = true;
        }
        for (auto y : range(ymin, ymax + 1)) {
            good[{ xmin, y }] = true;
            good[{ xmax, y }] = true;
        }
    }

    auto is_good = [&](i64 x, i64 y) -> bool {
        if (good.contains({ x, y })) {
            return good[{ x, y }];
        }
        auto c = 0;
        for (auto i : range(coords.size() - 1)) {
            auto j = i + 1;
            auto [x1, y1] = coords[i];
            auto [x2, y2] = coords[j];

            auto [xmin, xmax] = minmax(x1, x2);
            auto [ymin, ymax] = minmax(y1, y2);
            if (xmin != xmax) {
                continue;
            }

            // Check all vertical lines to the right.
            if (xmax <= x) {
                continue;
            }
            if (y > ymin && y <= ymax) {
                c++;
            }
        }

        auto res = c % 2 == 1;
        good[{ x, y }] = res;
        return res;
    };

    for (auto i : range(coords.size() - 2)) {
        for (auto j : range(i + 1, coords.size() - 1)) {
            auto [x1, y1] = coords[i];
            auto [x2, y2] = coords[j];
            auto a = (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1);
            if (a <= s) {
                continue;
            }

            auto [xmin, xmax] = minmax(x1, x2);
            auto [ymin, ymax] = minmax(y1, y2);
            auto good = all_of(range(xmin, xmax + 1),
                               [&](auto x) {
                                   return is_good(x, ymin) && is_good(x, ymax);
                               }) &&
                        all_of(range(ymin, ymax + 1), [&](auto y) {
                            return is_good(xmin, y) && is_good(xmax, y);
                        });
            if (good) {
                s = max(s, a);
            }
        }
    }

    return s;
}
