#include <di/container/tree/tree_set.h>

#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 5, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto ints = di::Vector<di::Tuple<i64, i64>> {};
    for (auto [row, line] : enumerate(lines)) {
        if (line.empty()) {
            continue;
        }
        if (line.contains('-')) {
            auto [s, e] = split_two(Tsv(line), '-');
            ints.push_back(Tuple { uparse_int(s), uparse_int(e) });
            continue;
        }

        auto x = uparse_int(line);
        for (auto [ss, e] : ints) {
            if (between(x, ss, e)) {
                s++;
                break;
            }
        }
    }
    return s;
}

AOC_SOLUTION(2025, 5, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto ints = di::TreeSet<di::Tuple<i64, i64>> {};
    for (auto [row, line] : enumerate(lines)) {
        if (!line.contains('-')) {
            continue;
        }
        auto [sss, eee] = split_two(Tsv(line), '-');
        auto s = uparse_int(sss);
        auto e = uparse_int(eee);
        auto it = ints.begin();
        auto dumb = false;
        while (it != ints.end()) {
            auto& [ss, ee] = *it;
            if (s >= ss && e <= ee) {
                // useless
                dumb = true;
                break;
            }

            if ((s < ss && e >= ss) || (s >= ss && s <= ee)) {
                // Extend the front. Since this is the first interval in range
                // dw.
                di::get<0>(*it) = di::min(s, ss);
                break;
            }
            it++;
        }
        if (dumb) {
            continue;
        }

        if (it == ints.end()) {
            // Now look to extend from back.
            it = ints.begin();
            while (it != ints.end()) {
                auto& [ss, ee] = *it;
                if (s >= ss && s <= ee && e > ee) {
                    break;
                }
                it++;
            }
        }

        if (it == ints.end()) {
            // We got nothing - insert.
            ints.insert(Tuple { s, e });
            continue;
        }

        // Now we have an iterator and we extend current range.
        // This can delete useless ranges.
        di::get<1>(*it) = di::max(di::get<1>(*it), e);
        auto [ns, ne] = *it;
        auto oit = it;
        ++it;
        while (it != ints.end()) {
            auto& [ss, ee] = *it;
            if (ss >= ns && ss <= ne) {
                auto is_at_end = it == --ints.end();
                it = ints.erase(it);
                ne = di::get<1>(*oit) = di::max(di::get<1>(*oit), ee);
                if (is_at_end) {
                    // This works around a bug in my TreeSet<>
                    break;
                }
            } else {
                break;
            }
        }
    }

    for (auto [ss, ee] : ints) {
        s += ee - ss + 1;
    }
    return s;
}
