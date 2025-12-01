#include "di/container/algorithm/count.h"
#include "di/container/algorithm/count_if.h"
#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/greater.h"
#include "di/function/not_fn.h"
#include "di/parser/parse_unchecked.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2020, 7, a, i64) {
    auto lines = input | splitv("\n"_sv);

    struct Node {
        Vec<Tuple<i32, Ts>> children;
        i64 count = 0;
    };

    Map<Ts, Node> graph;

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto w = line | splitv(' ');
        auto l = clone(w[0]);
        l += w[1];

        auto& node = graph[l];

        auto xx = w | drop(4) | join_with(" "_tsv) | to<Ts>();
        for (auto const& rs : xx | splitv(", "_tsv)) {
            auto v = clone(rs) | splitv(' ');
            auto n = di::parse<i32>(v[0]);
            if (!n) {
                continue;
            }
            auto r = clone(v[1]);
            r += v[2];

            node.children.push_back(Tuple { *n, clone(r) });
        }
    }

    auto visited = Set<Ts> {};

    auto dfs = [&](this auto&& self, Tsv name) -> i64 {
        if (name == "shinygold"_tsv) {
            return 1;
        }

        auto& node = graph[name];
        for (auto const& [c, child] : node.children) {
            node.count += c * self(child);
        }
        return node.count;
    };

    for (auto const& key : keys(graph)) {
        dfs(key);
    }

    s = count_if(values(graph), greater(1_i64), &Node::count);
    return s;
}

AOC_SOLUTION(2020, 7, b, i64) {
    auto lines = input | splitv("\n"_sv);

    struct Node {
        Vec<Tuple<i32, Ts>> children;
        i64 count = 0;
    };

    Map<Ts, Node> graph;

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto w = line | splitv(' ');
        auto l = clone(w[0]);
        l += w[1];

        auto& node = graph[l];

        auto xx = w | drop(4) | join_with(" "_tsv) | to<Ts>();
        for (auto const& rs : xx | splitv(", "_tsv)) {
            auto v = clone(rs) | splitv(' ');
            auto n = di::parse<i32>(v[0]);
            if (!n) {
                continue;
            }
            auto r = clone(v[1]);
            r += v[2];

            node.children.push_back(Tuple { *n, clone(r) });
        }
    }

    auto visited = Set<Ts> {};

    auto dfs = [&](this auto&& self, Tsv name) -> i64 {
        auto& node = graph[name];
        if (visited.contains(name)) {
            return node.count;
        }
        visited.insert(name.to_owned());
        if (node.children.empty()) {
            return 0;
        }
        for (auto const& [c, child] : node.children) {
            auto x = self(child);
            node.count += c + c * x;
        }
        return node.count;
    };

    s = dfs("shinygold"_tsv);

    return s;
}
