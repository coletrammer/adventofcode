#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/pairwise_transform.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/greater.h"
#include "di/function/minus.h"
#include "di/function/not_fn.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 2, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto ns = all_nums(line, ' ');

        auto ds = ns | pairwise_transform(minus);

        if (all_of(ds, greater(0)) || all_of(ds, less(0))) {
            if (all_of(ds | transform(abs), between(1, 3))) {
                s++;
            }
        }
    }
    return s;
}

AOC_SOLUTION(2024, 2, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto ns = all_nums(line, ' ');

        for (auto i : range(ns.size())) {
            auto nss = clone(ns);
            nss.erase(nss.begin() + i);

            auto ds = nss | pairwise_transform(minus);

            if (all_of(ds, greater(0)) || all_of(ds, less(0))) {
                if (all_of(ds | transform(abs), between(1, 3))) {
                    s++;
                    break;
                }
            }
        }
    }
    return s;
}
