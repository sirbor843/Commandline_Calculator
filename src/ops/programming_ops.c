#include "ops/programming_ops.h"
#include "utils/utils.h"
#include "data_structs/stack.h"
#include "data_structs/graph.h"
#include "data_structs/linked_list.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// CONCEPT: Script functions that can be called by name
static CalculationResult script_sum(double *args, int n) {
    CalculationResult res = {0.0, false, ""};
    for (int i = 0; i < n; i++) {
        res.value += args[i];
    }
    return res;
}

static CalculationResult script_product(double *args, int n) {
    CalculationResult res = {1.0, false, ""};
    for (int i = 0; i < n; i++) {
        res.value *= args[i];
    }
    return res;
}

static CalculationResult script_max(double *args, int n) {
    CalculationResult res = {0.0, false, ""};
    if (n == 0) {
        res.is_error = true;
        strcpy(res.err_message, "No arguments provided.");
        return res;
    }
    res.value = args[0];
    for (int i = 1; i < n; i++) {
        if (args[i] > res.value) res.value = args[i];
    }
    return res;
}

static CalculationResult script_min(double *args, int n) {
    CalculationResult res = {0.0, false, ""};
    if (n == 0) {
        res.is_error = true;
        strcpy(res.err_message, "No arguments provided.");
        return res;
    }
    res.value = args[0];
    for (int i = 1; i < n; i++) {
        if (args[i] < res.value) res.value = args[i];
    }
    return res;
}

static CalculationResult script_avg(double *args, int n) {
    CalculationResult res = script_sum(args, n);
    if (!res.is_error && n > 0) {
        res.value /= n;
    }
    return res;
}

// CONCEPT: Function pointer dispatch table (Day 23)
static ScriptEntry script_table[] = {
    {"sum", script_sum},
    {"product", script_product},
    {"max", script_max},
    {"min", script_min},
    {"avg", script_avg},
    {NULL, NULL}
};

void programming_list_scripts(void) {
    printf("\n");
    printf("    %s╭─ Available Scripts ──────────────╮%s\n", COLOR_SKY, COLOR_RESET);
    for (int i = 0; script_table[i].name != NULL; i++) {
        printf("    %s│%s  %s▸%s %-30s %s│%s\n", 
               COLOR_SKY, COLOR_RESET, COLOR_MINT, COLOR_RESET, 
               script_table[i].name, COLOR_SKY, COLOR_RESET);
    }
    printf("    %s╰───────────────────────────────────╯%s\n", COLOR_SKY, COLOR_RESET);
    printf("    %sUsage:%s programming run <script> <args...>\n\n", STYLE_DIM, COLOR_RESET);
}

// CONCEPT: Function pointer lookup and invocation
void programming_run_script(const char *script_name) {
    for (int i = 0; script_table[i].name != NULL; i++) {
        if (strcmp(script_name, script_table[i].name) == 0) {
            log_info("Script found. Use: programming run <script> <args...>");
            return;
        }
    }
    log_error("Unknown script. Use 'programming list' to see available scripts.");
}

CalculationResult programming_execute(const char *cmd, double *args, int nargs) {
    CalculationResult res = {0.0, true, "Unknown script."};
    
    // CONCEPT: Function pointer lookup
    for (int i = 0; script_table[i].name != NULL; i++) {
        if (strcmp(cmd, script_table[i].name) == 0) {
            // CONCEPT: Calling function through pointer
            return script_table[i].func(args, nargs);
        }
    }
    
    return res;
}

static void print_graph_vertex(int v) {
    printf("%d ", v);
}

// CONCEPT: Integration and demonstration of Stack, Graph, and Linked List data structures
void programming_demo_ds(const char *type) {
    bool run_all = (!type || strlen(type) == 0 || strcmp(type, "all") == 0);
    
    if (run_all || strcmp(type, "stack") == 0) {
        printf("\n    %s╭─ Stack Demonstration (LIFO) ─────────────────╮%s\n", COLOR_PURPLE, COLOR_RESET);
        Stack s = { NULL };
        printf("    %s│%s  Pushing 10.5, 20.25, 30.75 onto stack...\n", COLOR_PURPLE, COLOR_RESET);
        stack_push(&s, 10.5);
        stack_push(&s, 20.25);
        stack_push(&s, 30.75);
        printf("    %s│%s  Popping: %s%g%s, %s%g%s, %s%g%s (LIFO order)\n",
               COLOR_PURPLE, COLOR_RESET,
               COLOR_GOLD, stack_pop(&s), COLOR_RESET,
               COLOR_GOLD, stack_pop(&s), COLOR_RESET,
               COLOR_GOLD, stack_pop(&s), COLOR_RESET);
        printf("    %s╰───────────────────────────────────────────────╯%s\n", COLOR_PURPLE, COLOR_RESET);
        stack_free(&s);
    }
    
    if (run_all || strcmp(type, "list") == 0) {
        printf("\n    %s╭─ Linked List Demonstration ───────────────────╮%s\n", COLOR_MINT, COLOR_RESET);
        Node *head = NULL;
        append_node(&head, 1.1);
        append_node(&head, 2.2);
        append_node(&head, 3.3);
        printf("    %s│%s  Appended elements: 1.1 -> 2.2 -> 3.3\n", COLOR_MINT, COLOR_RESET);
        printf("    %s│%s  Last value retrieved: %s%g%s\n", COLOR_MINT, COLOR_RESET, COLOR_GOLD, get_last_value(head), COLOR_RESET);
        printf("    %s╰───────────────────────────────────────────────╯%s\n", COLOR_MINT, COLOR_RESET);
        free_list(head);
    }
    
    if (run_all || strcmp(type, "graph") == 0) {
        printf("\n    %s╭─ Graph Demonstration (Adjacency List) ────────╮%s\n", COLOR_TEAL, COLOR_RESET);
        Graph *g = graph_create(4);
        if (g) {
            graph_add_edge(g, 0, 1, 1.0);
            graph_add_edge(g, 0, 2, 2.0);
            graph_add_edge(g, 1, 3, 3.0);
            graph_add_edge(g, 2, 3, 4.0);
            printf("    %s│%s  Vertices: %d, Edges: (0->1), (0->2), (1->3), (2->3)\n", COLOR_TEAL, COLOR_RESET, g->num_vertices);
            printf("    %s│%s  BFS Traversal from 0: ", COLOR_TEAL, COLOR_RESET);
            graph_bfs(g, 0, print_graph_vertex);
            printf("\n");
            printf("    %s│%s  DFS Traversal from 0: ", COLOR_TEAL, COLOR_RESET);
            graph_dfs(g, 0, print_graph_vertex);
            printf("\n");
            graph_free(g);
        }
        printf("    %s╰───────────────────────────────────────────────╯%s\n\n", COLOR_TEAL, COLOR_RESET);
    }
}
