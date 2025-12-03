#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 3, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto m = long(0);
        for (auto i : range(line.size() - 1)) {
            for (auto j : range(i + 1, line.size())) {
                auto ss = String();
                ss.push_back(line[i]);
                ss.push_back(line[j]);
                auto x = uparse_int(ss);
                m = di::max(x, m);
            }
        }
        s += m;
    }
    return s;
}

AOC_SOLUTION(2025, 3, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto memo = di::TreeMap<di::Tuple<usize, u32>, di::Optional<i64>> {};

    auto bt = [&](this auto&& bt, Tsv input, u32 rem) -> Optional<i64> {
        if (memo.contains({ input.size(), rem })) {
            return di::get<1>(*memo.find({ input.size(), rem }));
        }
        if (rem == 0) {
            return 0;
        }
        if (input.size() < rem) {
            return {};
        }

        auto skip = bt(input.substr(1), rem).value_or(0);
        auto with = bt(input.substr(1), rem - 1).value();
        auto v = i64(input[0] - '0');
        for (auto _ : range(rem - 1)) {
            v *= 10;
        }
        with += v;
        auto r = di::max(skip, with);
        memo.insert_or_assign({ input.size(), rem }, r);
        return r;
    };

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto x = bt(line, 12).value();
        s += x;
        memo.clear();
    }
    return s;
}
