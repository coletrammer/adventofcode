#include "di/container/algorithm/max.h"
#include "di/container/algorithm/minmax.h"
#include "di/container/algorithm/product.h"
#include "di/container/interface/empty.h"
#include "di/container/interface/erase.h"
#include "di/container/queue/queue.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/container/function.h"
#include "di/function/not_fn.h"
#include "di/function/ycombinator.h"
#include "di/random/concepts/uniform_random_bit_generator.h"
#include "di/random/distribution/uniform_int_distribution.h"
#include "di/random/generator/linear_congruential_engine.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

class UnionFind {
public:
    explicit UnionFind(i32 n) {
        m_parents = range(n) | to<Vector>();
        m_ranks.resize(n);
    }

    auto find(int i) -> i32 {
        if (m_parents[i] != i) {
            m_parents[i] = find(m_parents[i]);
        }
        return m_parents[i];
    }

    void union_(int x, int y) {
        auto xroot = find(x);
        auto yroot = find(y);

        if (m_ranks[xroot] < m_ranks[yroot]) {
            m_parents[xroot] = yroot;
        } else if (m_ranks[xroot] > m_ranks[yroot]) {
            m_parents[yroot] = xroot;
        } else {
            m_parents[yroot] = xroot;
            m_ranks[xroot]++;
        }
    }

private:
    Vector<i32> m_parents;
    Vector<i32> m_ranks;
};

AOC_SOLUTION(2023, 25, a, i64) {
    auto lines = input | splitv('\n');

    auto edges_set = TreeSet<Tuple<i32, i32>> {};
    auto labels = TreeMap<Tsv, i32> {};
    auto rlabels = TreeMap<i32, Tsv> {};

    auto s = 0;
    auto assign = [&](Tsv string) {
        if (labels.contains(string)) {
            return labels[string];
        }
        labels[string] = s;
        rlabels[s] = string;
        return s++;
    };

    for (auto [row, line] : enumerate(lines)) {
        auto [left, rhs] = split_two(line.view(), ": "_tsv);
        auto l = assign(left);
        for (auto right : split(rhs, ' ')) {
            auto r = assign(right);
            edges_set.insert({ l, r });
        }
    }

    for (auto i : range(1000)) {
        (void) i;

        auto rng = MinstdRand(i);
        auto uf = UnionFind { s };

        auto edges = edges_set | to<Vector>();
        auto edge_counts = i32(edges.size());
        auto vertices = s;
        while (vertices > 2) {
            auto rng_dist = UniformIntDistribution<> { 0, edge_counts - 1 };
            auto i = rng_dist(rng);

            auto [src, dest] = edges[i];
            auto s1 = uf.find(src);
            auto s2 = uf.find(dest);

            if (s1 == s2) {
                continue;
            }

            vertices--;
            uf.union_(s1, s2);
        }

        auto c = 0;
        for (auto [a, b] : edges) {
            auto s1 = uf.find(a);
            auto s2 = uf.find(b);
            if (s1 != s2) {
                ++c;
            }
        }

        if (c == 3) {
            auto m = TreeMap<i32, i32> {};
            for (auto v : range(s)) {
                m[uf.find(v)]++;
            }

            return product(values(m));
        }
    }

    return 0;
}
