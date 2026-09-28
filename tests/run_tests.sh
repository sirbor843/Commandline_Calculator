#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

pass_count=0
fail_count=0

run_case() {
    local name="$1"
    local input="$2"
    local expected="$3"

    local out
    out="$(printf "%b" "$input" | ./calculator 2>&1 || true)"

    if [[ "$out" == *"$expected"* ]]; then
        echo "[PASS] $name"
        pass_count=$((pass_count + 1))
    else
        echo "[FAIL] $name"
        echo "  expected snippet: $expected"
        fail_count=$((fail_count + 1))
    fi
}

run_case "basic addition" "basic add 2 3\nquit\n" "5"
run_case "solver roots spaced expression" "solver roots x^2 - 2 0 2\nquit\n" "1.41421"
run_case "solver newton raphson" "solver newton x^2-4 3\nquit\n" "2"
run_case "solver polynomial eval" "solver poly 2 1 -3 2\nquit\n" "0"
run_case "solver matrix trace" "solver trace 2 1 2 3 4\nquit\n" "5"
run_case "calculus integral spaced expression" "calculus integral x^2 + 1 0 1\nquit\n" "1.333"
run_case "expression trailing token rejected" "solver roots x^2 junk 0 2\nquit\n" "Error evaluating expression at bounds."
run_case "division by zero rejected" "graphical plot 1/0\nquit\n" "Could not evaluate expression"
run_case "complex division by zero rejected" "complex div 1+2i 0+0i\nquit\n" "Division by zero in complex numbers."
run_case "financial irr calculation" "financial irr -100 50 60\nquit\n" "6.3941"
run_case "statistical quartiles" "statistical quartiles 1 2 3 4 5 6 7\nquit\n" "Quartiles"
run_case "statistical correlation" "statistical correlation 1 2 3 4 2 4 6 8\nquit\n" "1"
run_case "converter to binary" "converter to_binary 10\nquit\n" "0b1010"
run_case "programming ds demo" "programming ds_demo all\nquit\n" "Stack Demonstration"
run_case "history last works" "basic add 4 6\nhistory last\nquit\n" "10"
run_case "history clear works" "basic add 1 1\nhistory clear\nhistory\nquit\n" "History is empty"
run_case "programming concepts command" "programming concepts\nquit\n" "C Concepts Covered"

echo
echo "Passed: $pass_count"
echo "Failed: $fail_count"

if [[ "$fail_count" -ne 0 ]]; then
    exit 1
fi
