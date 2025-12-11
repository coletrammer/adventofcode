#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 11, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto graph = TreeMap<Tsv, Vector<Tsv>> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [l, r] = split_two(line.view(), ':');
        for (auto s : split(r, ' ')) {
            if (!s.empty()) {
                graph[l].push_back(s.view());
            }
        }
    }

    auto vis = TreeSet<Tsv> {};
    auto dfs = [&](this auto&& dfs, Tsv n) -> i64 {
        if (n == "out"_tsv) {
            return 1;
        }
        if (vis.contains(n)) {
            return 0;
        }
        vis.insert(n);
        auto r = 0_i64;
        for (auto child : graph[n]) {
            r += dfs(child);
        }
        vis.erase(n);
        return r;
    };
    s = dfs("you"_tsv);
    return s;
}

AOC_SOLUTION(2025, 11, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto graph = TreeMap<Tsv, Vector<Tsv>> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [l, r] = split_two(line.view(), ':');
        for (auto s : split(r, ' ')) {
            if (!s.empty()) {
                graph[l].push_back(s.view());
            }
        }
    }

    auto vis = TreeSet<Tsv> {};
    auto memo = TreeMap<Tuple<Tsv, Tsv>, i64> {};
    auto dfs = [&](this auto&& dfs, Tsv n, Tsv e) -> i64 {
        if (n == e) {
            return 1;
        }
        if (vis.contains(n)) {
            return 0;
        }
        if (memo.contains({ n, e })) {
            return memo[{ n, e }];
        }
        vis.insert(n);
        auto r = 0_i64;
        for (auto child : graph[n]) {
            r += dfs(child, e);
        }
        vis.erase(n);
        memo[{ n, e }] = r;
        return r;
    };
    s = dfs("svr"_tsv, "dac"_tsv) * dfs("dac"_tsv, "fft"_tsv) * dfs("fft"_tsv, "out"_tsv) +
        dfs("svr"_tsv, "fft"_tsv) * dfs("fft"_tsv, "dac"_tsv) * dfs("dac"_tsv, "out"_tsv);
    return s;
}
