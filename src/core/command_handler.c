#include "command_handler.h"
#include "ops/basic_ops.h"
#include "ops/scientific_ops.h"
#include "ops/memory_ops.h"
#include "ops/statistical_ops.h"
#include "ops/converter_ops.h"
#include "ops/graphical_ops.h"
#include "ops/calculus_ops.h"
#include "ops/solver_ops.h"
#include "ops/extra_ops.h"
#include "ops/complex_ops.h"
#include "ops/financial_ops.h"
#include "ops/programming_ops.h"
#include "ops/history_ops.h"
#include "core/evaluation.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    const char *name;
    CommandType type;
    CommandHandlerFunc handler;
} CommandEntry;

void handle_basic(const char *args);
void handle_scientific(const char *args);
void handle_memory(const char *args);
void handle_statistical(const char *args);
void handle_converter(const char *args);
void handle_graphical(const char *args);
void handle_calculus(const char *args);
void handle_solver(const char *args);
void handle_extra(const char *args);
void handle_complex(const char *args);
void handle_financial(const char *args);
void handle_programming(const char *args);
void handle_history(const char *args);

static CommandEntry command_table[] = {
    {"basic", CMD_BASIC, handle_basic},
    {"scientific", CMD_SCIENTIFIC, handle_scientific},
    {"memory", CMD_MEMORY, handle_memory},
    {"statistical", CMD_STATISTICAL, handle_statistical},
    {"converter", CMD_CONVERTER, handle_converter},
    {"graphical", CMD_GRAPHICAL, handle_graphical},
    {"calculus", CMD_CALCULUS, handle_calculus},
    {"solver", CMD_SOLVER, handle_solver},
    {"extra", CMD_EXTRA, handle_extra},
    {"complex", CMD_COMPLEX, handle_complex},
    {"financial", CMD_FINANCIAL, handle_financial},
    {"programming", CMD_PROGRAMMING, handle_programming},
    {"history", CMD_HISTORY, handle_history},
    {"help", CMD_HELP, NULL},
    {"quit", CMD_QUIT, NULL},
    {NULL, CMD_UNKNOWN, NULL}
};

static char *pop_last_token(char *text) {
    if (!text) return NULL;
    trim_whitespace(text);
    size_t len = strlen(text);
    if (len == 0) return NULL;

    char *end = text + len - 1;
    while (end > text && *end == ' ') {
        *end = '\0';
        end--;
    }

    char *last_space = strrchr(text, ' ');
    if (!last_space) {
        return text;
    }

    *last_space = '\0';
    char *token = last_space + 1;
    trim_whitespace(text);
    return token;
}

static int parse_calculus_integral_args(char *remaining, char **expr, double *a, double *b, int *n) {
    if (!remaining || !expr || !a || !b || !n) return 0;
    trim_whitespace(remaining);
    if (strlen(remaining) == 0) return 0;

    char attempt1[MAX_CMD_LEN];
    char attempt2[MAX_CMD_LEN];
    strncpy(attempt1, remaining, sizeof(attempt1) - 1);
    attempt1[sizeof(attempt1) - 1] = '\0';
    strncpy(attempt2, remaining, sizeof(attempt2) - 1);
    attempt2[sizeof(attempt2) - 1] = '\0';

    // Attempt A: <expr> <a> <b>
    char *b_tok_a = pop_last_token(attempt1);
    char *a_tok_a = pop_last_token(attempt1);
    double a_a = 0.0, b_a = 0.0;
    trim_whitespace(attempt1);
    int ok_a = (b_tok_a && a_tok_a && strlen(attempt1) > 0 &&
                sscanf(a_tok_a, "%lf", &a_a) == 1 &&
                sscanf(b_tok_a, "%lf", &b_a) == 1);
    CalculationResult expr_check_a = {0.0, true, ""};
    if (ok_a) {
        expr_check_a = evaluate_expression(attempt1, (a_a + b_a) / 2.0);
    }

    // Attempt B: <expr> <a> <b> <n>
    double a_b = 0.0, b_b = 0.0;
    int n_b = 0;
    char *n_tok_b = pop_last_token(attempt2);
    char *b_tok_b = pop_last_token(attempt2);
    char *a_tok_b = pop_last_token(attempt2);
    trim_whitespace(attempt2);
    int ok_b = (n_tok_b && b_tok_b && a_tok_b && strlen(attempt2) > 0 &&
                sscanf(a_tok_b, "%lf", &a_b) == 1 &&
                sscanf(b_tok_b, "%lf", &b_b) == 1 &&
                sscanf(n_tok_b, "%d", &n_b) == 1 && n_b > 0);
    CalculationResult expr_check_b = {0.0, true, ""};
    if (ok_b) {
        expr_check_b = evaluate_expression(attempt2, (a_b + b_b) / 2.0);
    }

    if (ok_a && !expr_check_a.is_error) {
        *expr = remaining;
        // rebuild chosen attempt into remaining buffer
        strncpy(remaining, attempt1, MAX_CMD_LEN - 1);
        remaining[MAX_CMD_LEN - 1] = '\0';
        *a = a_a;
        *b = b_a;
        *n = 1000;
        return 1;
    }

    if (ok_b && !expr_check_b.is_error) {
        strncpy(remaining, attempt2, MAX_CMD_LEN - 1);
        remaining[MAX_CMD_LEN - 1] = '\0';
        *expr = remaining;
        *a = a_b;
        *b = b_b;
        *n = n_b;
        return 1;
    }

    return 0;
}

CommandType identify_command(const char *cmd_name) {
    if (!cmd_name) return CMD_UNKNOWN;
    for (int i = 0; command_table[i].name != NULL; i++) {
        if (strcmp(cmd_name, command_table[i].name) == 0) {
            return command_table[i].type;
        }
    }
    return CMD_UNKNOWN;
}

void handle_command(const char *command_str) {
    char cmd_line[MAX_CMD_LEN];
    strncpy(cmd_line, command_str, MAX_CMD_LEN - 1);
    cmd_line[MAX_CMD_LEN - 1] = '\0';  // Ensure null-termination
    char *cmd_name = strtok(cmd_line, " ");
    if (!cmd_name) return;
    char *args = strtok(NULL, "");
    for (int i = 0; command_table[i].name != NULL; i++) {
        if (strcmp(cmd_name, command_table[i].name) == 0) {
            if (command_table[i].handler != NULL) {
                command_table[i].handler(args);
                return;
            } else if (command_table[i].type == CMD_HELP) {
                print_help();
                return;
            }
        }
    }
    log_error("Unknown command. Type 'help'.");
}

void handle_basic(const char *args) {
    if (!args) { log_error("Usage: basic <op> <values...>"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: basic <op> <values...>"); return; }
    
    // Multi-value operations
    if (strcmp(op, "sum") == 0 || strcmp(op, "product") == 0 || 
        strcmp(op, "avg") == 0 || strcmp(op, "min") == 0 || strcmp(op, "max") == 0) {
        int cap = 100, count = 0;
        double *values = malloc(cap * sizeof(double));
        if (!values) { log_error("Memory allocation failed"); return; }
        
        char *t = strtok(NULL, " ");
        while (t) {
            if (count >= cap) {
                cap *= 2;
                double *new_values = realloc(values, cap * sizeof(double));
                if (!new_values) { free(values); log_error("Memory allocation failed"); return; }
                values = new_values;
            }
            if (sscanf(t, "%lf", &values[count]) == 1) count++;
            t = strtok(NULL, " ");
        }
        
        if (count == 0) { free(values); log_error("No values provided"); return; }
        
        CalculationResult res;
        if (strcmp(op, "sum") == 0) res = basic_sum(values, count);
        else if (strcmp(op, "product") == 0) res = basic_product(values, count);
        else if (strcmp(op, "avg") == 0) res = basic_avg(values, count);
        else if (strcmp(op, "min") == 0) res = basic_min(values, count);
        else if (strcmp(op, "max") == 0) res = basic_max(values, count);
        else { free(values); return; }
        
        print_result(res);
        free(values);
        return;
    }
    
    // Single value operations
    char *v1s = strtok(NULL, " ");
    if (!v1s) { log_error("Usage: basic <op> <value(s)>"); return; }
    double v1;
    if (sscanf(v1s, "%lf", &v1) != 1) { log_error("Invalid number format"); return; }
    
    if (strcmp(op, "abs") == 0) { print_result(basic_abs(v1)); return; }
    if (strcmp(op, "sqrt") == 0) { print_result(basic_sqrt(v1)); return; }
    
    // Two value operations
    char *v2s = strtok(NULL, " ");
    if (!v2s) { log_error("Usage: basic <op> <v1> <v2>"); return; }
    double v2;
    if (sscanf(v2s, "%lf", &v2) != 1) { log_error("Invalid number format"); return; }
    
    CalculationResult res;
    if (strcmp(op, "add") == 0) res = basic_add(v1, v2);
    else if (strcmp(op, "sub") == 0) res = basic_sub(v1, v2);
    else if (strcmp(op, "mul") == 0) res = basic_mul(v1, v2);
    else if (strcmp(op, "div") == 0) res = basic_div(v1, v2);
    else if (strcmp(op, "mod") == 0) res = basic_mod(v1, v2);
    else if (strcmp(op, "pow") == 0) res = basic_pow(v1, v2);
    else if (strcmp(op, "percent") == 0) res = basic_percent(v1, v2);
    else { log_error("Unknown basic operation"); return; }
    print_result(res);
}

void handle_scientific(const char *args) {
    if (!args) { log_error("Usage: scientific <op> <value> [options]"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " "), *v1s = strtok(NULL, " "), *v2s = strtok(NULL, " ");
    if (!op) { log_error("Usage: scientific <op> <value> [options]"); return; }

    if (strcmp(op, "rand") == 0) { print_result(scientific_rand()); return; }

    if (!v1s) { log_error("Usage: scientific <op> <value> [options]"); return; }
    double v1;
    if (sscanf(v1s, "%lf", &v1) != 1) { log_error("Invalid number format"); return; }

    CalculationResult res;

    // Trig functions (support deg option)
    bool is_deg = (v2s && strcmp(v2s, "deg") == 0);
    if (strcmp(op, "sin") == 0) { print_result(scientific_sin(v1, is_deg)); return; }
    if (strcmp(op, "cos") == 0) { print_result(scientific_cos(v1, is_deg)); return; }
    if (strcmp(op, "tan") == 0) { print_result(scientific_tan(v1, is_deg)); return; }

    // Inverse trig (always return radians)
    if (strcmp(op, "asin") == 0) { print_result(scientific_asin(v1)); return; }
    if (strcmp(op, "acos") == 0) { print_result(scientific_acos(v1)); return; }
    if (strcmp(op, "atan") == 0) { print_result(scientific_atan(v1)); return; }

    // Hyperbolic
    if (strcmp(op, "sinh") == 0) { print_result(scientific_sinh(v1)); return; }
    if (strcmp(op, "cosh") == 0) { print_result(scientific_cosh(v1)); return; }
    if (strcmp(op, "tanh") == 0) { print_result(scientific_tanh(v1)); return; }
    if (strcmp(op, "asinh") == 0) { print_result(scientific_asinh(v1)); return; }
    if (strcmp(op, "acosh") == 0) { print_result(scientific_acosh(v1)); return; }
    if (strcmp(op, "atanh") == 0) { print_result(scientific_atanh(v1)); return; }

    // Logarithms and exponential
    if (strcmp(op, "log") == 0 || strcmp(op, "ln") == 0) { print_result(scientific_log(v1)); return; }
    if (strcmp(op, "log10") == 0) { print_result(scientific_log10(v1)); return; }
    if (strcmp(op, "log2") == 0) { print_result(scientific_log2(v1)); return; }
    if (strcmp(op, "log_base") == 0) {
        if (!v2s) { log_error("Usage: scientific log_base <value> <base>"); return; }
        double base;
        if (sscanf(v2s, "%lf", &base) != 1) { log_error("Invalid base"); return; }
        print_result(scientific_log_base(v1, base));
        return;
    }
    if (strcmp(op, "exp") == 0) { print_result(scientific_exp(v1)); return; }    
    // Roots
    if (strcmp(op, "sqrt") == 0) { print_result(scientific_sqrt(v1)); return; }
    if (strcmp(op, "cbrt") == 0) { print_result(scientific_cbrt(v1)); return; }
    if (strcmp(op, "nroot") == 0) {
        if (!v2s) { log_error("Usage: scientific nroot <value> <n>"); return; }
        double n;
        if (sscanf(v2s, "%lf", &n) != 1) { log_error("Invalid root"); return; }
        print_result(scientific_nroot(v1, n));
        return;
    }
    
    // Factorial
    if (strcmp(op, "fact") == 0) { print_result(scientific_factorial((int)v1)); return; }
    
    // Permutations and combinations
    if (strcmp(op, "perm") == 0 || strcmp(op, "nPr") == 0) {
        if (!v2s) { log_error("Usage: scientific perm <n> <r>"); return; }
        int r;
        if (sscanf(v2s, "%d", &r) != 1) { log_error("Invalid r value"); return; }
        print_result(scientific_permutation((int)v1, r));
        return;
    }
    if (strcmp(op, "comb") == 0 || strcmp(op, "nCr") == 0) {
        if (!v2s) { log_error("Usage: scientific comb <n> <r>"); return; }
        int r;
        if (sscanf(v2s, "%d", &r) != 1) { log_error("Invalid r value"); return; }
        print_result(scientific_combination((int)v1, r));
        return;
    }
    
    // Number theory
    if (strcmp(op, "gcd") == 0) {
        if (!v2s) { log_error("Usage: scientific gcd <a> <b>"); return; }
        long long b;
        if (sscanf(v2s, "%lld", &b) != 1) { log_error("Invalid number"); return; }
        print_result(scientific_gcd((long long)v1, b));
        return;
    }
    if (strcmp(op, "lcm") == 0) {
        if (!v2s) { log_error("Usage: scientific lcm <a> <b>"); return; }
        long long b;
        if (sscanf(v2s, "%lld", &b) != 1) { log_error("Invalid number"); return; }
        print_result(scientific_lcm((long long)v1, b));
        return;
    }
    if (strcmp(op, "isprime") == 0) {
        res = scientific_isprime((long long)v1);
        if (res.value == 1) log_success("Prime");
        else log_info("Not prime");
        return;
    }
    
    // Rounding
    if (strcmp(op, "floor") == 0) { print_result(scientific_floor(v1)); return; }
    if (strcmp(op, "ceil") == 0) { print_result(scientific_ceil(v1)); return; }
    if (strcmp(op, "round") == 0) { print_result(scientific_round(v1)); return; }
    
    // Angle conversion
    if (strcmp(op, "deg2rad") == 0) { print_result(scientific_deg2rad(v1)); return; }
    if (strcmp(op, "rad2deg") == 0) { print_result(scientific_rad2deg(v1)); return; }
    
    log_error("Unknown scientific operation");
}

static void memory_list_callback(const char *name, double val) {
    printf("  • %-10s = %10g\n", name, val);
}

void handle_memory(const char *args) {
    if (!args) { log_error("Usage: memory <store|recall|list> [name] [value]"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: memory <store|recall|list> [name] [value]"); return; }
    
    if (strcmp(op, "store") == 0) {
        char *name = strtok(NULL, " ");
        char *vs = strtok(NULL, " ");
        if (!name || !vs) { log_error("Usage: memory store <name> <value>"); return; }
        double v;
        if (sscanf(vs, "%lf", &v) != 1) { log_error("Invalid number format"); return; }
        memory_store(name, v);
        log_success("Variable stored.");
    } else if (strcmp(op, "recall") == 0) {
        char *name = strtok(NULL, " ");
        if (!name) { log_error("Usage: memory recall <name>"); return; }
        print_result(memory_recall(name));
    } else if (strcmp(op, "list") == 0) {
        printf("\n  ╭── Stored Variables ───╮\n");
        memory_list(memory_list_callback);
        printf("  ╰───────────────────────╯\n\n");
    } else {
        log_error("Unknown memory operation. Use: store, recall, list");
    }
}

void handle_statistical(const char *args) {
    if (!args) { log_error("Usage: statistical <op> <values...>"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: statistical <op> <values...>"); return; }
    
    // Percentile requires special handling
    double percentile_val = 0;
    if (strcmp(op, "percentile") == 0) {
        char *pval = strtok(NULL, " ");
        if (!pval || sscanf(pval, "%lf", &percentile_val) != 1) {
            log_error("Usage: statistical percentile <p> <values...>");
            return;
        }
    }
    
    int cap = 100, count = 0;
    double *data = malloc(cap * sizeof(double));
    if (!data) { log_error("Memory allocation failed"); return; }
    char *t = strtok(NULL, " ");
    while (t) {
        if (count >= cap) {
            cap *= 2;
            double *new_data = realloc(data, cap * sizeof(double));
            if (!new_data) { free(data); log_error("Memory allocation failed"); return; }
            data = new_data;
        }
        if (sscanf(t, "%lf", &data[count]) == 1) count++;
        t = strtok(NULL, " ");
    }
    if (count == 0) { free(data); log_error("No valid numbers provided"); return; }
    
    CalculationResult res;
    if (strcmp(op, "mean") == 0) res = statistical_mean(data, count);
    else if (strcmp(op, "median") == 0) res = statistical_median(data, count);
    else if (strcmp(op, "mode") == 0) res = statistical_mode(data, count);
    else if (strcmp(op, "stddev") == 0) res = statistical_stddev(data, count);
    else if (strcmp(op, "variance") == 0) res = statistical_variance(data, count);
    else if (strcmp(op, "range") == 0) res = statistical_range(data, count);
    else if (strcmp(op, "sum") == 0) res = statistical_sum(data, count);
    else if (strcmp(op, "min") == 0) res = statistical_min(data, count);
    else if (strcmp(op, "max") == 0) res = statistical_max(data, count);
    else if (strcmp(op, "count") == 0) res = statistical_count(data, count);
    else if (strcmp(op, "percentile") == 0) res = statistical_percentile(data, count, percentile_val);
    else if (strcmp(op, "quartiles") == 0) {
        double q1 = 0, q2 = 0, q3 = 0;
        res = statistical_quartiles(data, count, &q1, &q2, &q3);
        if (res.is_error) {
            log_error(res.err_message);
        } else {
            printf("\n    %s╭─ Quartiles (25%%, 50%%, 75%%) ─────╮%s\n", COLOR_PURPLE, COLOR_RESET);
            printf("    %s│%s  Q1 (25%%): %s%12.6g%s              %s│%s\n",
                   COLOR_PURPLE, COLOR_RESET, COLOR_GOLD, q1, COLOR_RESET, COLOR_PURPLE, COLOR_RESET);
            printf("    %s│%s  Q2 (50%%): %s%12.6g%s (Median)     %s│%s\n",
                   COLOR_PURPLE, COLOR_RESET, COLOR_GOLD, q2, COLOR_RESET, COLOR_PURPLE, COLOR_RESET);
            printf("    %s│%s  Q3 (75%%): %s%12.6g%s              %s│%s\n",
                   COLOR_PURPLE, COLOR_RESET, COLOR_GOLD, q3, COLOR_RESET, COLOR_PURPLE, COLOR_RESET);
            printf("    %s│%s  IQR:      %s%12.6g%s              %s│%s\n",
                   COLOR_PURPLE, COLOR_RESET, COLOR_MINT, q3 - q1, COLOR_RESET, COLOR_PURPLE, COLOR_RESET);
            printf("    %s╰───────────────────────────────────╯%s\n\n", COLOR_PURPLE, COLOR_RESET);
            history_add(q2);
        }
        free(data);
        return;
    } else if (strcmp(op, "covariance") == 0 || strcmp(op, "correlation") == 0 || strcmp(op, "regression") == 0) {
        if (count % 2 != 0 || count < 4) {
            free(data);
            log_error("Paired statistics require an even number of values (at least 2 pairs: x1..xn y1..yn)");
            return;
        }
        int n = count / 2;
        double *x = data;
        double *y = data + n;
        if (strcmp(op, "covariance") == 0) {
            res = statistical_covariance(x, y, n);
        } else if (strcmp(op, "correlation") == 0) {
            res = statistical_correlation(x, y, n);
        } else {
            CalculationResult slope_res = statistical_linear_regression_slope(x, y, n);
            CalculationResult int_res = statistical_linear_regression_intercept(x, y, n);
            if (slope_res.is_error || int_res.is_error) {
                log_error(slope_res.is_error ? slope_res.err_message : int_res.err_message);
            } else {
                printf("\n    %s╭─ Linear Regression ──────────────╮%s\n", COLOR_PURPLE, COLOR_RESET);
                printf("    %s│%s  Model:     y = %s%.4gx + %.4g%s     %s│%s\n",
                       COLOR_PURPLE, COLOR_RESET, COLOR_GOLD, slope_res.value, int_res.value, COLOR_RESET, COLOR_PURPLE, COLOR_RESET);
                printf("    %s│%s  Slope (m): %s%12.6g%s            %s│%s\n",
                       COLOR_PURPLE, COLOR_RESET, COLOR_MINT, slope_res.value, COLOR_RESET, COLOR_PURPLE, COLOR_RESET);
                printf("    %s│%s  Intercept: %s%12.6g%s            %s│%s\n",
                       COLOR_PURPLE, COLOR_RESET, COLOR_MINT, int_res.value, COLOR_RESET, COLOR_PURPLE, COLOR_RESET);
                printf("    %s╰───────────────────────────────────╯%s\n\n", COLOR_PURPLE, COLOR_RESET);
                history_add(slope_res.value);
            }
            free(data);
            return;
        }
    } else {
        free(data);
        log_error("Unknown statistical operation. Try: mean, median, mode, stddev, variance, range, quartiles, covariance, correlation, regression");
        return;
    }
    
    print_result(res);
    free(data);
}

void handle_converter(const char *args) {
    if (!args) { log_error("Usage: converter <length|temp|check_bit|to_binary> ..."); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " "), *v1s = strtok(NULL, " "), *v2s = strtok(NULL, " "), *v3s = strtok(NULL, " ");
    if (!op || !v1s) { log_error("Usage: converter <op> <value> ..."); return; }
    
    if (strcmp(op, "check_bit") == 0) {
        int v1;
        if (sscanf(v1s, "%d", &v1) != 1) { log_error("Invalid integer format"); return; }
        if (!v2s) { log_error("Usage: converter check_bit <value> <bit>"); return; }
        int bit;
        if (sscanf(v2s, "%d", &bit) != 1) { log_error("Invalid bit position"); return; }
        print_result(converter_check_bit(v1, bit));
    } else if (strcmp(op, "to_binary") == 0) {
        int v1;
        if (sscanf(v1s, "%d", &v1) != 1) { log_error("Invalid integer format"); return; }
        char bin_str[64];
        converter_format_binary(v1, bin_str, sizeof(bin_str));
        printf("\n    %s╭─ Binary Output ─────────────────╮%s\n", COLOR_TEAL, COLOR_RESET);
        printf("    %s│%s  Decimal: %s%-10d%s              %s│%s\n",
               COLOR_TEAL, COLOR_RESET, COLOR_GOLD, v1, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
        printf("    %s│%s  Binary:  %s0b%-20s%s  %s│%s\n",
               COLOR_TEAL, COLOR_RESET, COLOR_MINT, bin_str, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
        printf("    %s╰─────────────────────────────────╯%s\n\n", COLOR_TEAL, COLOR_RESET);
        CalculationResult res = converter_to_binary(v1);
        history_add(res.value);
        return;
    } else if (strcmp(op, "length") == 0) {
        // converter length 100 cm m
        if (!v2s || !v3s) { log_error("Usage: converter length <value> <from> <to>"); return; }
        double val;
        if (sscanf(v1s, "%lf", &val) != 1) { log_error("Invalid number format"); return; }
        print_result(converter_length(val, v2s, v3s));
    } else if (strcmp(op, "temp") == 0) {
        // converter temp 100 C F
        if (!v2s || !v3s) { log_error("Usage: converter temp <value> <from> <to>"); return; }
        double val;
        if (sscanf(v1s, "%lf", &val) != 1) { log_error("Invalid number format"); return; }
        print_result(converter_temperature(val, v2s, v3s));
    } else {
        log_error("Unknown converter operation. Use: length, temp, check_bit, to_binary");
    }
}

void handle_graphical(const char *args) {
    if (!args) { log_error("Usage: graphical plot <expression>"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: graphical plot <expression>"); return; }
    if (strcmp(op, "plot") == 0) {
        char *expr = strtok(NULL, "");
        if (expr) graphical_plot(expr);
        else log_error("Usage: graphical plot <expression>");
    } else {
        log_error("Unknown graphical operation. Use: plot");
    }
}

void handle_calculus(const char *args) {
    if (!args) { log_error("Usage: calculus integral <expr> <a> <b> [n]"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: calculus integral <expr> <a> <b> [n]"); return; }
    if (strcmp(op, "integral") == 0) {
        char *remaining = strtok(NULL, "");
        if (!remaining) { log_error("Usage: calculus integral <expr> <a> <b> [n]"); return; }

        char expr_buf[MAX_CMD_LEN];
        strncpy(expr_buf, remaining, MAX_CMD_LEN - 1);
        expr_buf[MAX_CMD_LEN - 1] = '\0';

        char *expr = expr_buf;
        double a, b;
        int n = 1000;
        if (!parse_calculus_integral_args(expr_buf, &expr, &a, &b, &n)) {
            log_error("Usage: calculus integral <expr> <a> <b> [n]");
            return;
        }
        print_result(calculus_integral(expr, a, b, n));
    } else {
        log_error("Unknown calculus operation. Use: integral");
    }
}

void handle_solver(const char *args) {
    if (!args) { log_error("Usage: solver <op> <values...>"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: solver <op> <values...>"); return; }
    
    if (strcmp(op, "det2x2") == 0 || strcmp(op, "matrix_det") == 0) {
        double m[2][2];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                char *s = strtok(NULL, " ");
                if (!s) { log_error("Usage: solver det2x2 <a> <b> <c> <d>"); return; }
                if (sscanf(s, "%lf", &m[i][j]) != 1) { log_error("Invalid number"); return; }
            }
        }
        print_result(solver_matrix_det2x2(m));
        return;
    }
    
    if (strcmp(op, "det3x3") == 0) {
        double m[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                char *s = strtok(NULL, " ");
                if (!s) { log_error("Usage: solver det3x3 <9 values row-major>"); return; }
                if (sscanf(s, "%lf", &m[i][j]) != 1) { log_error("Invalid number"); return; }
            }
        }
        print_result(solver_matrix_det3x3(m));
        return;
    }
    
    if (strcmp(op, "quadratic") == 0) {
        char *as = strtok(NULL, " "), *bs = strtok(NULL, " "), *cs = strtok(NULL, " ");
        if (!as || !bs || !cs) { log_error("Usage: solver quadratic <a> <b> <c>"); return; }
        double a, b, c;
        if (sscanf(as, "%lf", &a) != 1 || sscanf(bs, "%lf", &b) != 1 || sscanf(cs, "%lf", &c) != 1) {
            log_error("Invalid coefficients"); return;
        }
        QuadraticResult qres = solver_quadratic(a, b, c);
        if (!qres.has_solution) {
            log_error(qres.message);
        } else if (qres.is_complex) {
            printf("\n    %s╭─────────────────────────────────────╮%s\n", COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  %sComplex roots:%s                       %s│%s\n", 
                   COLOR_TEAL, COLOR_RESET, COLOR_PURPLE, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  x₁ = %s%.4f + %.4fi%s               %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, qres.x1, qres.x2, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  x₂ = %s%.4f - %.4fi%s               %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, qres.x1, qres.x2, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s╰─────────────────────────────────────╯%s\n\n", COLOR_TEAL, COLOR_RESET);
        } else {
            printf("\n    %s╭─────────────────────────────────────╮%s\n", COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  %sReal roots:%s                          %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_MINT, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  x₁ = %s%14.6g%s                  %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, qres.x1, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  x₂ = %s%14.6g%s                  %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, qres.x2, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s╰─────────────────────────────────────╯%s\n\n", COLOR_TEAL, COLOR_RESET);
        }
        return;
    }
    
    if (strcmp(op, "linear2") == 0) {
        double a1, b1, c1, a2, b2, c2;
        char *s;
        s = strtok(NULL, " "); if (!s || sscanf(s, "%lf", &a1) != 1) { log_error("Invalid input"); return; }
        s = strtok(NULL, " "); if (!s || sscanf(s, "%lf", &b1) != 1) { log_error("Invalid input"); return; }
        s = strtok(NULL, " "); if (!s || sscanf(s, "%lf", &c1) != 1) { log_error("Invalid input"); return; }
        s = strtok(NULL, " "); if (!s || sscanf(s, "%lf", &a2) != 1) { log_error("Invalid input"); return; }
        s = strtok(NULL, " "); if (!s || sscanf(s, "%lf", &b2) != 1) { log_error("Invalid input"); return; }
        s = strtok(NULL, " "); if (!s || sscanf(s, "%lf", &c2) != 1) { log_error("Invalid input"); return; }
        
        LinearSystem2Result lres = solver_linear_system_2x2(a1, b1, c1, a2, b2, c2);
        if (!lres.has_solution) {
            log_error(lres.message);
        } else {
            printf("\n    %s╭─────────────────────────────────────╮%s\n", COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  %sSolution:%s                            %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_MINT, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  x = %s%14.6g%s                   %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, lres.x, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  y = %s%14.6g%s                   %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, lres.y, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s╰─────────────────────────────────────╯%s\n\n", COLOR_TEAL, COLOR_RESET);
        }
        return;
    }
    
    if (strcmp(op, "linear3") == 0) {
        double coeffs[3][4];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                char *s = strtok(NULL, " ");
                if (!s || sscanf(s, "%lf", &coeffs[i][j]) != 1) {
                    log_error("Usage: solver linear3 <12 values: a1 b1 c1 d1 a2 b2 c2 d2 a3 b3 c3 d3>");
                    return;
                }
            }
        }
        LinearSystem3Result lres = solver_linear_system_3x3(coeffs);
        if (!lres.has_solution) {
            log_error(lres.message);
        } else {
            printf("\n    %s╭─────────────────────────────────────╮%s\n", COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  %sSolution:%s                            %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_MINT, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  x = %s%14.6g%s                   %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, lres.x, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  y = %s%14.6g%s                   %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, lres.y, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s│%s  z = %s%14.6g%s                   %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, COLOR_GOLD, lres.z, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("    %s╰─────────────────────────────────────╯%s\n\n", COLOR_TEAL, COLOR_RESET);
        }
        return;
    }
    
    if (strcmp(op, "roots") == 0) {
        char *remaining = strtok(NULL, "");
        if (!remaining) {
            log_error("Usage: solver roots <expr> <a> <b>"); 
            return; 
        }
        trim_whitespace(remaining);
        char *bs = pop_last_token(remaining);
        char *as = pop_last_token(remaining);
        char *expr = remaining;
        if (!expr || !as || !bs || strlen(expr) == 0) {
            log_error("Usage: solver roots <expr> <a> <b>");
            return;
        }
        double a, b;
        if (sscanf(as, "%lf", &a) != 1 || sscanf(bs, "%lf", &b) != 1) {
            log_error("Invalid bounds"); return;
        }
        print_result(solver_bisection(expr, a, b, 1e-10, 100));
        return;
    }
    
    if (strcmp(op, "newton") == 0) {
        char *remaining = strtok(NULL, "");
        if (!remaining) { log_error("Usage: solver newton <expr> <x0>"); return; }
        trim_whitespace(remaining);
        char *x0s = pop_last_token(remaining);
        char *expr = remaining;
        if (!expr || !x0s || strlen(expr) == 0) {
            log_error("Usage: solver newton <expr> <x0>");
            return;
        }
        double x0;
        if (sscanf(x0s, "%lf", &x0) != 1) { log_error("Invalid initial guess"); return; }
        print_result(solver_newton_raphson(expr, x0, 1e-10, 100));
        return;
    }

    if (strcmp(op, "poly") == 0) {
        char *xs = strtok(NULL, " ");
        if (!xs) { log_error("Usage: solver poly <x> <coeffs...>"); return; }
        double x;
        if (sscanf(xs, "%lf", &x) != 1) { log_error("Invalid x value"); return; }
        
        int cap = 10, count = 0;
        double *coeffs = malloc(cap * sizeof(double));
        if (!coeffs) { log_error("Memory allocation failed"); return; }
        
        char *t = strtok(NULL, " ");
        while (t) {
            if (count >= cap) {
                cap *= 2;
                double *new_coeffs = realloc(coeffs, cap * sizeof(double));
                if (!new_coeffs) { free(coeffs); log_error("Memory allocation failed"); return; }
                coeffs = new_coeffs;
            }
            if (sscanf(t, "%lf", &coeffs[count]) == 1) count++;
            t = strtok(NULL, " ");
        }
        if (count == 0) { free(coeffs); log_error("No coefficients provided"); return; }
        print_result(solver_poly_eval(coeffs, count - 1, x));
        free(coeffs);
        return;
    }

    if (strcmp(op, "trace") == 0) {
        char *ns = strtok(NULL, " ");
        if (!ns) { log_error("Usage: solver trace <size_n> <n*n matrix values...>"); return; }
        int n;
        if (sscanf(ns, "%d", &n) != 1 || n <= 0) { log_error("Invalid matrix size"); return; }
        double *m = malloc(n * n * sizeof(double));
        if (!m) { log_error("Memory allocation failed"); return; }
        for (int i = 0; i < n * n; i++) {
            char *s = strtok(NULL, " ");
            if (!s || sscanf(s, "%lf", &m[i]) != 1) {
                free(m);
                log_error("Insufficient matrix elements provided.");
                return;
            }
        }
        print_result(solver_matrix_trace(m, n));
        free(m);
        return;
    }
    
    log_error("Unknown solver operation. Try: det2x2, det3x3, quadratic, linear2, linear3, roots, newton, poly, trace");
}

void handle_extra(const char *args) {
    if (!args) { log_error("Usage: extra save|load|save_bin|load_bin ..."); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: extra save|load|save_bin|load_bin ..."); return; }
    if (strcmp(op, "save") == 0) {
        char *vs = strtok(NULL, " "), *fs = strtok(NULL, " ");
        if (!vs || !fs) { log_error("Usage: extra save <value> <filename>"); return; }
        double val;
        if (sscanf(vs, "%lf", &val) != 1) { log_error("Invalid number format"); return; }
        CalculationResult res = extra_save_result(val, fs);
        if (!res.is_error) log_success("Result saved to file.");
        else log_error(res.err_message);
    } else if (strcmp(op, "load") == 0) {
        char *fs = strtok(NULL, " ");
        if (!fs) { log_error("Usage: extra load <filename>"); return; }
        print_result(extra_load_data(fs));
    } else if (strcmp(op, "save_bin") == 0) {
        char *fs = strtok(NULL, " ");
        if (!fs) { log_error("Usage: extra save_bin <filename> <values...>"); return; }
        int cap = 10, count = 0;
        double *data = malloc(cap * sizeof(double));
        if (!data) { log_error("Memory allocation failed"); return; }
        char *t = strtok(NULL, " ");
        while (t) {
            if (count >= cap) {
                cap *= 2;
                double *new_data = realloc(data, cap * sizeof(double));
                if (!new_data) { free(data); log_error("Memory allocation failed"); return; }
                data = new_data;
            }
            if (sscanf(t, "%lf", &data[count]) == 1) count++;
            t = strtok(NULL, " ");
        }
        if (count == 0) { free(data); log_error("No values provided to save."); return; }
        CalculationResult res = extra_save_binary(data, count, fs);
        free(data);
        if (!res.is_error) log_success("Binary data saved successfully.");
        else log_error(res.err_message);
    } else if (strcmp(op, "load_bin") == 0) {
        char *fs = strtok(NULL, " ");
        if (!fs) { log_error("Usage: extra load_bin <filename>"); return; }
        double buffer[256];
        int out_n = 0;
        CalculationResult res = extra_load_binary(buffer, 256, fs, &out_n);
        if (res.is_error) {
            log_error(res.err_message);
        } else {
            printf("\n    %s╭─ Loaded %d Binary Values ────────╮%s\n", COLOR_MINT, out_n, COLOR_RESET);
            for (int i = 0; i < out_n && i < 10; i++) {
                printf("    %s│%s  [%d] = %s%12.6g%s             %s│%s\n",
                       COLOR_MINT, COLOR_RESET, i, COLOR_GOLD, buffer[i], COLOR_RESET, COLOR_MINT, COLOR_RESET);
            }
            if (out_n > 10) printf("    %s│%s  ... and %d more items             %s│%s\n", COLOR_MINT, COLOR_RESET, out_n - 10, COLOR_MINT, COLOR_RESET);
            printf("    %s╰──────────────────────────────────╯%s\n\n", COLOR_MINT, COLOR_RESET);
            CalculationResult sum_res = basic_sum(buffer, out_n);
            if (!sum_res.is_error) history_add(sum_res.value);
        }
    } else {
        log_error("Unknown extra operation. Use: save, load, save_bin, load_bin");
    }
}

// CONCEPT: Complex number handler
void handle_complex(const char *args) {
    if (!args) { log_error("Usage: complex <add|sub|mul|div> <a+bi> <c+di>"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " "), *z1s = strtok(NULL, " "), *z2s = strtok(NULL, " ");
    if (!op) { log_error("Usage: complex <add|sub|mul|div> <a+bi> <c+di>"); return; }
    
    if (strcmp(op, "mag") == 0) {
        if (!z1s) { log_error("Usage: complex mag <a+bi>"); return; }
        Complex z1;
        CalculationResult parse = complex_parse(z1s, &z1);
        if (parse.is_error) { log_error(parse.err_message); return; }
        CalculationResult res = {complex_magnitude(z1), false, ""};
        print_result(res);
        return;
    }
    
    if (!z1s || !z2s) { log_error("Usage: complex <add|sub|mul|div> <a+bi> <c+di>"); return; }
    
    Complex z1, z2, result;
    CalculationResult parse1 = complex_parse(z1s, &z1);
    CalculationResult parse2 = complex_parse(z2s, &z2);
    
    if (parse1.is_error) { log_error(parse1.err_message); return; }
    if (parse2.is_error) { log_error(parse2.err_message); return; }
    
    if (strcmp(op, "add") == 0) result = complex_add(z1, z2);
    else if (strcmp(op, "sub") == 0) result = complex_sub(z1, z2);
    else if (strcmp(op, "mul") == 0) result = complex_mul(z1, z2);
    else if (strcmp(op, "div") == 0) {
        if (fabs(z2.real) < 1e-15 && fabs(z2.imag) < 1e-15) {
            log_error("Division by zero in complex numbers.");
            return;
        }
        result = complex_div(z1, z2);
    }
    else { log_error("Unknown operation. Use: add, sub, mul, div, mag"); return; }
    
    complex_print(result);
}

// CONCEPT: Financial operations handler
void handle_financial(const char *args) {
    if (!args) { log_error("Usage: financial <npv|compound> ..."); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: financial <npv|compound> ..."); return; }
    
    if (strcmp(op, "npv") == 0) {
        // financial npv <rate> <cf0> <cf1> ...
        char *rates = strtok(NULL, " ");
        if (!rates) { log_error("Usage: financial npv <rate> <cashflows...>"); return; }
        double rate;
        if (sscanf(rates, "%lf", &rate) != 1) { log_error("Invalid rate"); return; }
        
        int cap = 10, count = 0;
        double *cfs = malloc(cap * sizeof(double));
        if (!cfs) { log_error("Memory allocation failed"); return; }
        
        char *t = strtok(NULL, " ");
        while (t) {
            if (count >= cap) {
                cap *= 2;
                double *new_cfs = realloc(cfs, cap * sizeof(double));
                if (!new_cfs) { free(cfs); log_error("Memory allocation failed"); return; }
                cfs = new_cfs;
            }
            if (sscanf(t, "%lf", &cfs[count]) == 1) count++;
            t = strtok(NULL, " ");
        }
        
        if (count == 0) { free(cfs); log_error("No cashflows provided"); return; }
        print_result(financial_npv(rate, cfs, count));
        free(cfs);
    } else if (strcmp(op, "compound") == 0) {
        // financial compound <principal> <rate> <periods>
        char *ps = strtok(NULL, " "), *rs = strtok(NULL, " "), *ns = strtok(NULL, " ");
        if (!ps || !rs || !ns) { log_error("Usage: financial compound <principal> <rate> <periods>"); return; }
        double principal, rate;
        int periods;
        if (sscanf(ps, "%lf", &principal) != 1) { log_error("Invalid principal"); return; }
        if (sscanf(rs, "%lf", &rate) != 1) { log_error("Invalid rate"); return; }
        if (sscanf(ns, "%d", &periods) != 1) { log_error("Invalid periods"); return; }
        print_result(financial_compound_interest(principal, rate, periods));
    } else if (strcmp(op, "pmt") == 0) {
        char *rs = strtok(NULL, " "), *ns = strtok(NULL, " "), *pvs = strtok(NULL, " ");
        if (!rs || !ns || !pvs) { log_error("Usage: financial pmt <rate> <periods> <present_value>"); return; }
        double rate, pv;
        int periods;
        if (sscanf(rs, "%lf", &rate) != 1 || sscanf(ns, "%d", &periods) != 1 || sscanf(pvs, "%lf", &pv) != 1) { log_error("Invalid arguments"); return; }
        print_result(financial_pmt(rate, periods, pv));
    } else if (strcmp(op, "fv") == 0) {
        char *rs = strtok(NULL, " "), *ns = strtok(NULL, " "), *pmts = strtok(NULL, " "), *pvs = strtok(NULL, " ");
        if (!rs || !ns || !pmts || !pvs) { log_error("Usage: financial fv <rate> <periods> <pmt> <present_value>"); return; }
        double rate, pmt, pv;
        int periods;
        if (sscanf(rs, "%lf", &rate) != 1 || sscanf(ns, "%d", &periods) != 1 || sscanf(pmts, "%lf", &pmt) != 1 || sscanf(pvs, "%lf", &pv) != 1) { log_error("Invalid arguments"); return; }
        print_result(financial_fv(rate, periods, pmt, pv));
    } else if (strcmp(op, "pv") == 0) {
        char *rs = strtok(NULL, " "), *ns = strtok(NULL, " "), *pmts = strtok(NULL, " "), *fvs = strtok(NULL, " ");
        if (!rs || !ns || !pmts || !fvs) { log_error("Usage: financial pv <rate> <periods> <pmt> <future_value>"); return; }
        double rate, pmt, fv;
        int periods;
        if (sscanf(rs, "%lf", &rate) != 1 || sscanf(ns, "%d", &periods) != 1 || sscanf(pmts, "%lf", &pmt) != 1 || sscanf(fvs, "%lf", &fv) != 1) { log_error("Invalid arguments"); return; }
        print_result(financial_pv(rate, periods, pmt, fv));
    } else if (strcmp(op, "irr") == 0) {
        int cap = 10, count = 0;
        double *cfs = malloc(cap * sizeof(double));
        if (!cfs) { log_error("Memory allocation failed"); return; }
        
        char *t = strtok(NULL, " ");
        while (t) {
            if (count >= cap) {
                cap *= 2;
                double *new_cfs = realloc(cfs, cap * sizeof(double));
                if (!new_cfs) { free(cfs); log_error("Memory allocation failed"); return; }
                cfs = new_cfs;
            }
            if (sscanf(t, "%lf", &cfs[count]) == 1) count++;
            t = strtok(NULL, " ");
        }
        if (count < 2) { free(cfs); log_error("Usage: financial irr <cashflows...> (requires at least 2 cash flows)"); return; }
        print_result(financial_irr(cfs, count));
        free(cfs);
    } else {
        log_error("Unknown operation. Use: npv, irr, compound, pmt, fv, pv");
    }
}

// CONCEPT: Programming/scripting handler with function pointers
void handle_programming(const char *args) {
    if (!args) { log_error("Usage: programming <list|run|concepts|ds_demo> [script] [args...]"); return; }
    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) { log_error("Usage: programming <list|run|concepts|ds_demo> [script] [args...]"); return; }
    
    if (strcmp(op, "list") == 0) {
        programming_list_scripts();
    } else if (strcmp(op, "concepts") == 0) {
        printf("\n");
        printf("    %s╭─ C Concepts Covered (Snapshot) ──────────────╮%s\n", COLOR_SKY, COLOR_RESET);
        printf("    %s│%s  %s▸%s C99, preprocessor, enums, unions     %s│%s\n", COLOR_SKY, COLOR_RESET, COLOR_MINT, COLOR_RESET, COLOR_SKY, COLOR_RESET);
        printf("    %s│%s  %s▸%s pointers, dynamic memory, recursion   %s│%s\n", COLOR_SKY, COLOR_RESET, COLOR_MINT, COLOR_RESET, COLOR_SKY, COLOR_RESET);
        printf("    %s│%s  %s▸%s structs, function pointers, callbacks %s│%s\n", COLOR_SKY, COLOR_RESET, COLOR_MINT, COLOR_RESET, COLOR_SKY, COLOR_RESET);
        printf("    %s│%s  %s▸%s queues, stacks, BST, graph structures %s│%s\n", COLOR_SKY, COLOR_RESET, COLOR_MINT, COLOR_RESET, COLOR_SKY, COLOR_RESET);
        printf("    %s╰───────────────────────────────────────────────╯%s\n\n", COLOR_SKY, COLOR_RESET);
    } else if (strcmp(op, "ds_demo") == 0) {
        char *type = strtok(NULL, " ");
        programming_demo_ds(type);
    } else if (strcmp(op, "run") == 0) {
        char *script = strtok(NULL, " ");
        if (!script) { log_error("Usage: programming run <script> [args...]"); return; }
        
        int cap = 10, count = 0;
        double *args_arr = malloc(cap * sizeof(double));
        if (!args_arr) { log_error("Memory allocation failed"); return; }
        
        char *t = strtok(NULL, " ");
        while (t) {
            if (count >= cap) {
                cap *= 2;
                double *new_args = realloc(args_arr, cap * sizeof(double));
                if (!new_args) { free(args_arr); log_error("Memory allocation failed"); return; }
                args_arr = new_args;
            }
            if (sscanf(t, "%lf", &args_arr[count]) == 1) count++;
            t = strtok(NULL, " ");
        }
        
        print_result(programming_execute(script, args_arr, count));
        free(args_arr);
    } else {
        log_error("Unknown operation. Use: list, run, concepts, ds_demo");
    }
}

void handle_history(const char *args) {
    if (!args || strlen(args) == 0) {
        history_show();
        return;
    }

    char copy[MAX_CMD_LEN];
    strncpy(copy, args, MAX_CMD_LEN - 1);
    copy[MAX_CMD_LEN - 1] = '\0';
    char *op = strtok(copy, " ");
    if (!op) {
        history_show();
        return;
    }

    if (strcmp(op, "show") == 0) {
        history_show();
    } else if (strcmp(op, "clear") == 0) {
        history_clear();
        log_success("History cleared.");
    } else if (strcmp(op, "last") == 0) {
        print_result(history_last());
    } else {
        log_error("Unknown history operation. Use: show, clear, last");
    }
}
