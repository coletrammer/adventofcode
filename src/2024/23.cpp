#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 23, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto g = Map<Ts, Set<Ts>> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [a, b] = split_two(line.view(), '-');
        g[a].insert(b.to_owned());
        g[b].insert(a.to_owned());
    }

    Set<Set<Ts>> res;
    for (auto const& key : keys(g)) {
        if (!key.starts_with('t')) {
            continue;
        }

        for (auto const& a : g[key]) {
            for (auto const& b : g[key]) {
                if (a <= b) {
                    continue;
                }

                if (g[a].contains(b)) {
                    auto set = Set<Ts> {};
                    set.insert(clone(key));
                    set.insert(clone(a));
                    set.insert(clone(b));
                    res.insert(di::move(set));
                }
            }
        }
    }
    return res.size();
}

AOC_SOLUTION(2024, 23, b, Ts) {
    auto lines = input | splitv("\n"_tsv);

    auto g = Map<Ts, Set<Ts>> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [a, b] = split_two(line.view(), '-');
        g[a].insert(b.to_owned());
        g[b].insert(a.to_owned());
    }

    Set<Tsv> res;
    Set<Set<Tsv>> memo;
    auto dfs = ycombinator([&](auto&& dfs, Tsv node, Set<Tsv>& vis) -> void {
        if (memo.contains(vis)) {
            return;
        }
        memo.insert(clone(vis));

        if (vis.size() > res.size()) {
            res = clone(vis);
        }

        for (auto const& cand : g[node]) {
            if (vis.contains(cand)) {
                continue;
            }

            auto const& new_edges = g[cand];
            if (!all_of(vis, [&](Tsv x) {
                    return new_edges.contains(x);
                })) {
                continue;
            }

            vis.insert(cand.view());
            dfs(cand.view(), vis);
            vis.erase(cand.view());
        }
    });

    for (auto const& key : keys(g)) {
        auto vis = Set<Tsv> {};
        vis.insert(key);
        dfs(key, vis);
    }

    return join_with(res, ',') | to<Ts>();
}
