#include <di/container/queue/priority_queue.h>
#include <di/container/view/cartesian_product.h>
#include <di/function/compare_backwards.h>

#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

using Point = di::Tuple<i32, i32, i32>;

class UnionFind {
public:
    UnionFind(di::Vector<Point> const& xs) {
        for (auto p : xs) {
            m_parents[p] = p;
        }
    }

    auto find(Point i) -> Point {
        if (m_parents[i] != i) {
            m_parents[i] = find(m_parents[i]);
        }
        return m_parents[i];
    }

    void union_(Point x, Point y) {
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
    TreeMap<Point, Point> m_parents;
    TreeMap<Point, i32> m_ranks;
};

auto dist(Point p1, Point p2) -> i64 {
    auto [x1, y1, z1] = p1;
    auto [x2, y2, z2] = p2;

    auto d = i64(x1 - x2) * (x1 - x2) + i64(y1 - y2) * (y1 - y2) + i64(z1 - z2) * (z1 - z2);
    return d;
}

auto dist(di::Tuple<Point, Point> p) -> i64 {
    return dist(di::get<0>(p), di::get<1>(p));
}

struct Comp {
    auto operator()(di::Tuple<Point, Point> a, di::Tuple<Point, Point> b) const { return dist(a) <=> dist(b); }
};

AOC_SOLUTION(2025, 8, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto xs = di::Vector<di::Tuple<i32, i32, i32>> {};
    for (auto [row, line] : enumerate(lines)) {
        auto xx = all_nums(line, ',');
        if (xx.size() < 3) {
            continue;
        }
        xs.emplace_back(xx[0], xx[1], xx[2]);
    }

    auto pairs = di::PriorityQueue<di::Tuple<Point, Point>, di::Vector<di::Tuple<Point, Point>>, Comp> {};
    auto const N = is_test ? 10u : 1000u;
    for (auto i : range(xs.size() - 1)) {
        for (auto j : range(i + 1, xs.size())) {
            auto p1 = xs[i];
            auto p2 = xs[j];
            if (pairs.size() < N) {
                pairs.push({ p1, p2 });
            } else if (dist(p1, p2) < dist(pairs.top().value())) {
                pairs.pop();
                pairs.push({ p1, p2 });
            }
        }
    }

    auto uf = UnionFind(xs);
    for (auto [p1, p2] : pairs) {
        uf.union_(p1, p2);
    }

    auto freq = di::TreeMap<Point, i32> {};
    for (auto p : xs) {
        freq[uf.find(p)]++;
    }

    auto res = freq | di::values | di::to<di::Vector>();
    di::sort(res, di::compare_backwards);
    return res | di::take(3) | di::product;
}

AOC_SOLUTION(2025, 8, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto xs = di::Vector<di::Tuple<i32, i32, i32>> {};
    for (auto [row, line] : enumerate(lines)) {
        auto xx = all_nums(line, ',');
        if (xx.size() < 3) {
            continue;
        }
        xs.emplace_back(xx[0], xx[1], xx[2]);
    }

    auto pairs = di::PriorityQueue<di::Tuple<Point, Point>, di::Vector<di::Tuple<Point, Point>>, Comp> {};
    auto const N = xs.size() * xs.size();
    for (auto i : range(xs.size() - 1)) {
        for (auto j : range(i + 1, xs.size())) {
            auto p1 = xs[i];
            auto p2 = xs[j];
            if (pairs.size() < N) {
                pairs.push({ p1, p2 });
            } else if (dist(p1, p2) < dist(pairs.top().value())) {
                pairs.pop();
                pairs.push({ p1, p2 });
            }
        }
    }

    auto uf = UnionFind(xs);
    for (auto [p1, p2] : pairs | di::to<di::Vector>() | di::reverse) {
        uf.union_(p1, p2);

        auto freq = di::TreeMap<Point, i32> {};
        for (auto p : xs) {
            freq[uf.find(p)]++;
        }
        if (freq.size() == 1) {
            return (i64) di::get<0>(p1) * di::get<0>(p2);
        }
    }

    return 0;
}
