#include "dius/main.h"

#include "di/cli/parser.h"
#include "di/container/path/path_view.h"
#include "di/container/string/string.h"
#include "di/container/string/transparent_encoding.h"
#include "di/format/present.h"
#include "di/format/style.h"
#include "di/io/read_all.h"
#include "di/io/read_to_string.h"
#include "di/types/byte.h"
#include "dius/print.h"
#include "dius/sync_file.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

namespace aoc {
struct Args {
    di::Optional<di::PathView> input;
    int year { 2025 };
    int day { 1 };
    bool part_b { false };
    di::Optional<i32> test;
    bool verbose { false };
    bool list { false };
    bool help { false };

    constexpr static auto get_cli_parser() {
        return di::cli_parser<Args>("aoc"_sv, "Advent of Code Solver"_sv)
            .option<&Args::input>('i', "input"_tsv, "Input file path"_sv)
            .option<&Args::year>('y', "year"_tsv, "Year to solve"_sv)
            .option<&Args::day>('d', "day"_tsv, "Day to solve"_sv)
            .option<&Args::part_b>('b', "part-b"_tsv, "Run part b"_sv)
            .option<&Args::test>('t', "test"_tsv, "Run with test input file number"_sv)
            .option<&Args::list>('l', "list"_tsv, "List all available solutions"_sv)
            .option<&Args::verbose>('v', "verbose"_tsv, "Print verbose information for some solutions"_sv)
            .help();
    }
};

di::Result<void> list_solutions() {
    struct Problem {
        bool a { false };
        bool b { false };
    };

    auto list = di::TreeMap<int, di::TreeMap<int, Problem>> {};
    for (auto const& [year, day, is_b] : AocProblemRegistry::the().list()) {
        if (is_b) {
            list[year][day].b = true;
        } else {
            list[year][day].a = true;
        }
    }

    for (auto const& [year, days] : list) {
        dius::println("{}"_sv, di::Styled(di::Styled(year, di::FormatEffect::Bold), di::FormatEffect::Underline));
        for (auto const& [day, problem] : days) {
            dius::println("  {:2} ({})"_sv, day, problem.b && problem.a ? "a,b"_sv : problem.a ? "a"_sv : "b"_sv);
        }
        dius::println(""_sv);
    }
    return {};
}

di::Result<void> main(Args& args) {
    if (args.list) {
        return list_solutions();
    }

    auto file_name = args.test.has_value() ? TRY(di::present("test_{:02}.txt"_sv, *args.test)) : "input.txt"_s;
    auto default_path_string = TRY(di::present("input/{}/{:02}/{}"_sv, args.year, args.day, file_name));
    auto default_path_transparent_string = default_path_string | di::transform([](c32 code_point) {
                                               return static_cast<char>(code_point);
                                           }) |
                                           di::to<di::Vector>() | di::to<di::TransparentString>();
    auto default_path = di::Path { di::move(default_path_transparent_string) };

    auto path = args.input.value_or(default_path);
    auto string = TRY(detail::read_to_string(path) | di::if_error([&](auto&& error) {
                          dius::eprintln("Failed to read input file '{}': {}"_sv, path.data(), error.message());
                      }));
    auto view = string.view();
    while (view.ends_with('\n')) {
        view = view.substr(view.begin(), view.end() - 1);
    }

    auto solver = AocProblemRegistry::the().lookup({ args.year, args.day, args.part_b });
    if (!solver) {
        dius::eprintln("No solver found for {} day {} part {}"_sv, args.year, args.day, args.part_b ? "b"_sv : "a"_sv);
        return di::Unexpected(di::BasicError::InvalidArgument);
    }

    (*solver)(view, args.test.has_value(), args.verbose);
    return {};
}
}

DIUS_MAIN(aoc::Args, aoc)
