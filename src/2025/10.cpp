#include <di/function/compare_backwards.h>
#include <di/math/gcd.h>

#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2025, 10, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto parts = line | splitv(' ');
        auto target_s = parts[0].view();
        target_s = target_s.substr(1, target_s.size() - 2);
        auto buttons_ss = parts.subspan(1, parts.size() - 2).value();

        auto desired = 0_i64;
        for (auto [i, ch] : target_s | enumerate) {
            if (ch == '#') {
                desired |= 1 << i;
            }
        }

        auto buttons = buttons_ss | transform([](auto const& ss) -> i64 {
                           auto s = ss.view();
                           s = s.substr(1, s.size() - 2);
                           auto ns = all_nums(s, ',');
                           auto r = 0_i64;
                           for (auto n : ns) {
                               r |= 1 << n;
                           }
                           return r;
                       }) |
                       di::to<di::Vector>();

        auto memo = TreeMap<i64, i64> {};
        auto vis = TreeSet<i64> {};
        auto bt = [&](this auto&& bt, i64 desired) -> i64 {
            if (memo.contains(desired)) {
                return memo[desired];
            }

            auto r = di::NumericLimits<i64>::max;
            for (auto b : buttons) {
                if (desired == b) {
                    return 1;
                }
                if (vis.contains(desired ^ b)) {
                    continue;
                }
                vis.insert(desired ^ b);
                auto x = bt(desired ^ b);
                if (x == di::NumericLimits<i64>::max) {
                    continue;
                }
                r = min(r, 1 + x);
                vis.erase(desired ^ b);
            }
            memo[desired] = r;
            return r;
        };

        auto r = bt(desired);
        s += r;
    }
    return s;
}

AOC_SOLUTION(2025, 10, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto parts = line | splitv(' ');
        auto joltage_s = parts.back().value().view();
        joltage_s = joltage_s.substr(1, joltage_s.size() - 2);
        auto buttons_ss = parts.subspan(1, parts.size() - 2).value();

        auto desired = all_nums_i<u16>(joltage_s, ',');

        using SVec = StaticVector<u16, Constexpr<10zu>>;
        auto buttons = buttons_ss | transform([&](auto const& ss) -> SVec {
                           auto s = ss.view();
                           s = s.substr(1, s.size() - 2);
                           auto ns = all_nums(s, ',');
                           auto r = SVec {};
                           ASSERT(r.resize(desired.size()));
                           for (auto n : ns) {
                               r[n] = 1;
                           }
                           return r;
                       }) |
                       di::to<di::Vector>();

        auto const N = desired.size();
        auto solve = [&](this auto&& solve, Span<u16> state, i64 bound, Vector<bool>& disallowed_buttons) -> i64 {
            if (bound < 0) {
                return NumericLimits<i64>::max;
            }
            if (di::container::equal(state, desired)) {
                return 0;
            }
            // GCD reduction.
            // auto gcd = di::Optional<u16> {};
            // for (auto i : range(N)) {
            //     auto x = desired[i] - state[i];
            //     if (x) {
            //         if (gcd) {
            //             gcd = di::gcd(gcd.value(), x);
            //         } else {
            //             gcd = x;
            //         }
            //     }
            // }
            // if (gcd.has_value() && gcd != 1_u16) {
            //     auto ss = SVec {};
            //     ASSERT(ss.append_container(state));
            //
            //     for (auto i : range(N)) {
            //         auto x = desired[i] - state[i];
            //         if (x) {
            //             x /= gcd.value();
            //             state[i] = desired[i] - x;
            //         }
            //     }
            //
            //     auto r = solve(state, (bound + gcd.value() - 1) / gcd.value(), disallowed_buttons);
            //     if (r != NumericLimits<i64>::max) {
            //         r *= gcd.value();
            //     }
            //
            //     copy(ss, state.begin());
            //
            //     return r;
            // }

            auto most_constrained = 0_usize;
            for (auto i : range(N)) {
                if (state[i] > desired[i]) {
                    return NumericLimits<i64>::max;
                } else if (state[i] < desired[i]) {
                    most_constrained = i;
                }
            }

            // Find most constrained thing not already solved.
            auto most_constrained_buttons = di::NumericLimits<int>::max;
            auto valid_buttons = StaticVector<usize, Constexpr<16zu>> {};
            for (auto i : range(N)) {
                if (state[i] == desired[i]) {
                    continue;
                }

                // Candidate, look at all buttons. We should've removed all invalid buttons at this point.
                auto btns = 0;
                auto vvalid_buttons = StaticVector<usize, Constexpr<16zu>> {};
                for (auto const& [bn, b] : buttons | enumerate) {
                    if (disallowed_buttons[bn]) {
                        continue;
                    }
                    if (b[i] != 1) {
                        continue;
                    }
                    btns++;
                    ASSERT(vvalid_buttons.push_back(bn));
                }
                if (btns == 0) {
                    // This is unsolveable, bail.
                    return NumericLimits<i64>::max;
                } else if (btns < most_constrained_buttons ||
                           (btns == most_constrained_buttons &&
                            (desired[i] - state[i]) < (desired[most_constrained] - state[most_constrained]))) {
                    most_constrained = i;
                    most_constrained_buttons = btns;
                    valid_buttons = di::move(vvalid_buttons);
                }
            }

            if (most_constrained_buttons == 1) {
                // Easy case - just allocate the button.
                auto times = desired[most_constrained] - state[most_constrained];
                for (auto i : range(N)) {
                    state[i] += times * buttons[valid_buttons[0]][i];
                }
                disallowed_buttons[valid_buttons[0]] = true;

                auto r = solve(state, bound - times, disallowed_buttons);
                if (r != NumericLimits<i64>::max) {
                    r += times;
                }

                // Restore
                for (auto i : range(N)) {
                    state[i] -= times * buttons[valid_buttons[0]][i];
                }
                disallowed_buttons[valid_buttons[0]] = false;

                return r;
            }

            // Try each button in sequence in any combo. Sort by number of things so we try
            // the easiest button first.
            auto r = NumericLimits<i64>::max;
            // valid_buttons = {};
            // for (auto [i, b] : disallowed_buttons | enumerate) {
            //     if (!b) {
            //         ASSERT(valid_buttons.push_back(i));
            //     }
            // }
            sort(valid_buttons, di::compare, [&](usize btn) {
                return sum(buttons[btn]);
            });
            for (auto const& button : valid_buttons) {
                // This is no longer a candidate for future solves, across all iters.
                disallowed_buttons[button] = true;

                auto max_times = NumericLimits<int>::max;
                for (auto i : range(N)) {
                    if (buttons[button][i]) {
                        max_times = min(max_times, desired[i] - state[i]);
                    }
                }
                if (max_times > bound) {
                    break;
                }
                auto min_times = 0;
                auto max_avail_other = 0;
                for (auto const& button : valid_buttons) {
                    if (!disallowed_buttons[button]) {
                        // Max avail is min() across all things where has affect.
                        auto for_button = max_times;
                        for (auto i : range(N)) {
                            if (buttons[button][i]) {
                                for_button = min(for_button, desired[i] - state[i]);
                            }
                        }
                        max_avail_other += for_button;
                    }
                }
                if (max_avail_other < max_times) {
                    min_times = max_times - max_avail_other;
                }
                if (min_times > max_times) {
                    continue;
                }
                for (auto times : range(min_times, max_times + 1)) {
                    auto done = false;
                    for (auto i : range(N)) {
                        state[i] += times * buttons[button][i];
                        if (state[i] > desired[i]) {
                            done = true;
                        }
                    }

                    // If unsolvable, ignore
                    auto rr = solve(state, min(bound, r) - times, disallowed_buttons);
                    if (rr != NumericLimits<i64>::max) {
                        r = min(times + rr, r);
                    }

                    // Restore
                    for (auto i : range(N)) {
                        state[i] -= times * buttons[button][i];
                    }
                    if (done) {
                        break;
                    }
                }
            }

            // Restore
            for (auto b : valid_buttons) {
                disallowed_buttons[b] = false;
            }
            return r;
        };
        // auto memo = TreeMap<Tuple<SVec, StaticVector<bool, Constexpr<16zu>>>, i64> {};
        // auto solve_memo = [&](this auto&& solve_memo, Span<u16> state, i64 bound,
        //                       Vector<bool>& disallowed_buttons) -> i64 {
        //     auto sstate = SVec {};
        //     for (auto s : state) {
        //         ASSERT(sstate.push_back(s));
        //     }
        //     auto db = StaticVector<bool, Constexpr<16zu>> {};
        //     for (auto b : disallowed_buttons) {
        //         ASSERT(db.push_back(b));
        //     }
        //     auto k = Tuple { sstate, db };
        //     if (memo.contains(k)) {
        //         return memo[k];
        //     }
        //     auto r = solve(solve_memo, state, bound, disallowed_buttons);
        //     memo[k] = r;
        //     return r;
        // };
        auto state_v = SVec {};
        ASSERT(state_v.resize(desired.size()));
        auto btns_set = Vector<bool> {};
        btns_set.resize(buttons.size());
        auto const r = solve(state_v.span(), NumericLimits<i64>::max, btns_set);
        s += r;
    }
    return s;
}
