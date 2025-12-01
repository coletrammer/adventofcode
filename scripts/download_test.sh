#!/usr/bin/env bash

usage() {
    echo "Usage: $0 <year> <day>"
    echo "NOTE: The AOC_SESSION environment variable must be set to your session cookie."
    exit 1
}

if [ $# -ne 2 ]; then
    usage
fi
if [ -z "$AOC_SESSION" ]; then
    usage
fi

script_dir=$(dirname "$0")
project_root=$(realpath "$script_dir/..")

year=$1
day=$2

output_dir="$(printf "input/%d/%02d" "$year" "$day")"
mkdir -p "$project_root/$output_dir"

url="https://adventofcode.com/$year/day/$day"

unescape() {
    while read -r lin; do
        newl=${lin//&gt;/>}
        newl=${newl//&lt;/<}
        newl=${newl//&amp;/<}
        echo "$newl"
    done
}

split_sections() {
    counter=0

    while IFS= read -r line; do
        if [[ $line =~ ^~+$ ]]; then
            file_path="$(printf "%s/%s/test_%02d.txt" "$project_root" "$output_dir" "$counter")"

            echo "Saving test input 'test_$(printf "%02d" "$counter")' to '$file_path'..."
            echo -n "$block" | unescape | sed -Ez '$ s/\n+$//' >"$file_path"

            counter=$((counter + 1))
            block=""
        else
            block="$block$line"$'\n'
        fi
    done
}

echo "Downloading test input from '$url' to '$project_root/$output_dir'..."
curl -s -b "session=$AOC_SESSION" "$url" |
    hxnormalize -x |
    hxselect -c -s '\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n' '.day-desc pre code' |
    split_sections
