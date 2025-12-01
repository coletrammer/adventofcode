#include "di/function/compare_backwards.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 16, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto srow = 0_usize;
    auto scol = 0_usize;
    auto erow = 0_usize;
    auto ecol = 0_usize;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch == 'S') {
                srow = row;
                scol = col;
            } else if (ch == 'E') {
                erow = row;
                ecol = col;
            }
        }
    }

    struct State {
        i64 score = 0;
        usize row = 0;
        usize col = 0;
        int dr = 0;
        int dc = 0;

        bool operator==(State const&) const = default;
        auto operator<=>(State const&) const = default;
    };

    struct State2 {
        usize row = 0;
        usize col = 0;
        int dr = 0;
        int dc = 0;

        bool operator==(State2 const&) const = default;
        auto operator<=>(State2 const&) const = default;
    };

    PriorityQueue<State, Vec<State>, CompareBackwards> queue;
    Set<State2> visited;

    auto add = [&](State state) {
        if (state.row >= lines.size() || state.col >= lines[0].size()) {
            return;
        }

        if (lines[state.row][state.col] == '#') {
            return;
        }

        auto state2 = State2 { state.row, state.col, state.dr, state.dc };
        if (!visited.contains(state2)) {
            queue.push(state);
            visited.insert(state2);
        }
    };

    add({ 0, srow, scol, 0, 1 }); // east

    while (!queue.empty()) {
        auto state = *queue.pop();
        if (state.row == erow && state.col == ecol) {
            return state.score;
        }

        // Turns
        add({ state.score + 1000, state.row, state.col, state.dc, state.dr });
        add({ state.score + 1000, state.row, state.col, -state.dc, -state.dr });

        add({ state.score + 1, state.row + state.dr, state.col + state.dc, state.dr, state.dc });
    }

    return s;
}

AOC_SOLUTION(2024, 16, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto srow = 0_usize;
    auto scol = 0_usize;
    auto erow = 0_usize;
    auto ecol = 0_usize;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch == 'S') {
                srow = row;
                scol = col;
            } else if (ch == 'E') {
                erow = row;
                ecol = col;
            }
        }
    }

    struct State {
        i64 score = 0;
        Set<Coord> tiles;
        usize row = 0;
        usize col = 0;
        int dr = 0;
        int dc = 0;

        bool operator==(State const& other) const { return this->score == other.score; }
        auto operator<=>(State const& other) const { return this->score <=> other.score; };
    };

    struct State2 {
        usize row = 0;
        usize col = 0;
        int dr = 0;
        int dc = 0;

        bool operator==(State2 const&) const = default;
        auto operator<=>(State2 const&) const = default;
    };

    MultiSet<State> queue;
    Map<State2, i64> scores;

    auto add = [&](State state) {
        if (state.row >= lines.size() || state.col >= lines[0].size()) {
            return;
        }

        if (lines[state.row][state.col] == '#') {
            return;
        }

        auto state2 = State2 { state.row, state.col, state.dr, state.dc };
        auto& score_map = scores[state2];
        if (score_map == 0 || state.score <= score_map) {
            state.tiles.insert({ state.row, state.col });
            queue.insert(di::move(state));
            score_map = state.score;
        }
    };

    add({ 1, {}, srow, scol, 0, 1 }); // east

    auto fscore = Optional<i64> {};
    auto fset = Set<Coord> {};
    while (!queue.empty()) {
        auto state = di::move(*queue.begin());
        queue.erase(queue.begin());
        if (state.row == erow && state.col == ecol) {
            if (fscore.has_value() && state.score > *fscore) {
                break;
            }

            fscore = state.score;
            for (auto tile : state.tiles) {
                fset.insert(tile);
            }
            continue;
        }
        if (fscore.has_value() && state.score >= fscore.value()) {
            break;
        }

        // Turns
        add({ state.score + 1000, clone(state.tiles), state.row, state.col, state.dc, state.dr });
        add({ state.score + 1000, clone(state.tiles), state.row, state.col, -state.dc, -state.dr });

        add({ state.score + 1, clone(state.tiles), state.row + state.dr, state.col + state.dc, state.dr, state.dc });
    }

    if (verbose) {
        for (auto r : range(lines.size())) {
            for (auto c : range(lines[r].size())) {
                auto ch = lines[r][c];
                if (fset.contains({ r, c })) {
                    ch = 'O';
                }
                print("{}"_sv, ch);
            }
            println(""_sv);
        }
    }

    return fset.size();
}
