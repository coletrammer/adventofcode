#include "di/function/container/prelude.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 24, a, i64) {
    auto [inputs, connections] = split_two(input, "\n\n"_tsv);

    auto get_gate_function = [&](Tsv gate) -> Function<bool(bool, bool)> {
        if (gate == "AND"_tsv) {
            return [](bool a, bool b) {
                return a && b;
            };
        }
        if (gate == "OR"_tsv) {
            return [](bool a, bool b) {
                return a || b;
            };
        }
        return [](bool a, bool b) {
            return a != b;
        };
    };

    auto gates = Vec<Tuple<Ts, Ts, Function<bool(bool, bool)>, Ts>> {};
    auto g = Map<Ts, Vec<usize>> {};
    for (auto line : connections | split("\n"_tsv)) {
        auto parts = splitv(line, ' ');
        gates.emplace_back(clone(parts[0]), clone(parts[2]), get_gate_function(parts[1]), clone(parts[4]));
        g[parts[0]].push_back(gates.size() - 1);
        g[parts[2]].push_back(gates.size() - 1);
    }

    auto queue = Queue<Ts> {};
    auto values = Map<Ts, bool> {};
    for (auto [row, line] : enumerate(inputs | split('\n'))) {
        auto [a, b] = split_two(line.view(), ": "_tsv);
        values[a] = uparse_int(b);
        queue.push(a.to_owned());
    }

    while (!queue.empty()) {
        auto node = *queue.pop();
        for (auto i : g[node]) {
            auto& [a, b, op, c] = gates[i];
            if (values.contains(a) && values.contains(b) && !values.contains(c)) {
                values[c] = op(values[a], values[b]);
                queue.push(clone(c));
            }
        }
    }

    auto st = ""_ts;
    for (auto const& [k, v] : values) {
        if (k.starts_with('z')) {
            if (v) {
                st.push_back('1');
            } else {
                st.push_back('0');
            }
        }
    }

    st.push_back('b');
    st.push_back('0');
    di::container::reverse(st.span());

    if (verbose) {
        println("{}"_sv, st);
    }
    return uparse_int(st);
}

AOC_SOLUTION(2024, 24, b, Ts) {
    auto [inputs, connections] = split_two(input, "\n\n"_tsv);

    auto get_gate_function = [&](Tsv gate) -> Function<bool(bool, bool)> {
        if (gate == "AND"_tsv) {
            return [](bool a, bool b) {
                return a && b;
            };
        }
        if (gate == "OR"_tsv) {
            return [](bool a, bool b) {
                return a || b;
            };
        }
        return [](bool a, bool b) {
            return a != b;
        };
    };

    auto gates = Vec<Tuple<Tsv, Tsv, Tsv, Function<bool(bool, bool)>, Tsv>> {};
    auto g = Map<Tsv, Vec<usize>> {};
    auto rg = Map<Tsv, usize> {};
    auto values = Map<Tsv, bool> {};
    auto vars = Set<Tsv> {};
    auto queue = Queue<Tsv> {};

    auto get_num = [&](char c) -> i64 {
        auto st = ""_ts;
        for (auto const& [k, v] : values) {
            if (k.starts_with(c)) {
                if (v) {
                    st.push_back('1');
                } else {
                    st.push_back('0');
                }
            }
        }

        st.push_back('b');
        st.push_back('0');
        di::container::reverse(st.span());

        return uparse_int(st);
    };

    for (auto line : connections | split("\n"_tsv)) {
        auto parts = split(line, ' ') | to<Vector>();
        gates.emplace_back(parts[0], parts[2], parts[1], get_gate_function(parts[1]), parts[4]);
        g[parts[0]].push_back(gates.size() - 1);
        g[parts[2]].push_back(gates.size() - 1);
        rg[parts[4]] = gates.size() - 1;

        vars.insert(parts[0]);
        vars.insert(parts[2]);
        vars.insert(parts[4]);
    }

    auto xxx = 0_i64;
    for (auto [row, line] : enumerate(inputs | split('\n'))) {
        auto [a, b] = split_two(line.view(), ": "_tsv);
        values[a] = bool(xxx++ % 2); // uparse_int(b);
        vars.insert(a);
        queue.push(a);
    }

    auto const x = get_num('x');
    auto const y = get_num('y');
    auto const z = x + y;

    auto bits = Map<Tsv, i64> {};

    auto vis = Set<Tsv> {};
    auto dfs = [&](this auto&& dfs, Tsv node) -> void {
        if (vis.contains(node)) {
            return;
        }
        vis.insert(node);

        if (node.starts_with('x') || node.starts_with('y')) {
            bits[node]++;
        }

        if (!rg.contains(node)) {
            return;
        }

        auto const& [a, b, _, _, _] = gates[rg[node]];
        dfs(a);
        dfs(b);
    };

    for (auto const& k : vars) {
        if (k.starts_with('z')) {
            vis.clear();
            dfs(k);
        }
    }

    println("{}"_sv, bits);

    auto swapped = Set<Tsv> {};
    auto swap_gates = [&](Tsv a, Tsv b) {
        swapped.insert(a);
        swapped.insert(b);

        auto a1 = find_if(gates, [&](auto const& gate) {
            return tget<4>(gate) == a;
        });
        auto a2 = find_if(gates, [&](auto const& gate) {
            return tget<4>(gate) == b;
        });
        swap(tget<4>(*a1), tget<4>(*a2));
    };

    swap_gates("z16"_tsv, "fkb"_tsv);
    swap_gates("z31"_tsv, "rdn"_tsv);
    swap_gates("z37"_tsv, "rrn"_tsv);
    swap_gates("nnr"_tsv, "rqf"_tsv);

    println("digraph comp {{"_sv);
    for (auto const& [a, b, n, _, c] : gates) {
        println("{} -> {};"_sv, a, c);
        println("{} -> {};"_sv, b, c);
        println("{} [label=\"{}\"];"_sv, c, n);
        if (c.starts_with('z')) {
            println("{} -> z{};"_sv, c, c);
        }
    }
    println("}}"_sv);

    while (!queue.empty()) {
        auto node = *queue.pop();
        for (auto i : g[node]) {
            auto& [a, b, _, op, c] = gates[i];
            if (values.contains(a) && values.contains(b) && !values.contains(c)) {
                values[c] = op(values[a], values[b]);
                queue.push(c);
            }
        }
    }
    auto const zz = get_num('z');
    println("x={} ({:b})"_sv, x, x);
    println("y={} ({:b})"_sv, y, y);
    println("z={} ({:b})"_sv, z, z);
    println("z={} ({:b})"_sv, zz, zz);

    return join_with(swapped, ',') | to<Ts>();
}
