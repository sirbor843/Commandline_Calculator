#include "common.h"
#include "command_handler.h"
#include "utils/utils.h"
#include "utils/signals.h"
#include "initialization.h"
#include "cleanup.h"
#include <stdio.h>
#include <string.h>

// CONCEPT: Function definition and modular design
void print_banner(void) {
    printf("\n");
    
    // Top border with gradient effect
    printf("  %s╭", GRAD_1);
    for (int i = 0; i < 62; i++) printf("─");
    printf("╮%s\n", COLOR_RESET);
    
    // Empty line for spacing
    printf("  %s│%s%62s%s│%s\n", GRAD_1, COLOR_RESET, "", GRAD_1, COLOR_RESET);
    
    // ASCII Art title with gradient colors
    printf("  %s│%s   %s █▀▀ █▀█ █   █▀▀ █ █ █   █▀█ ▀█▀ █▀█ █▀█ %s              %s│%s\n",
           GRAD_2, COLOR_RESET, COLOR_GOLD, COLOR_RESET, GRAD_2, COLOR_RESET);
    printf("  %s│%s   %s █   █▀█ █   █   █ █ █   █▀█  █  █ █ █▀▄ %s              %s│%s\n",
           GRAD_3, COLOR_RESET, COLOR_ORANGE, COLOR_RESET, GRAD_3, COLOR_RESET);
    printf("  %s│%s   %s █▄▄ █ █ █▄▄ █▄▄ █▄█ █▄▄ █ █  █  █▄█ █ █ %s              %s│%s\n",
           GRAD_4, COLOR_RESET, COLOR_CORAL, COLOR_RESET, GRAD_4, COLOR_RESET);
    
    // Empty line
    printf("  %s│%s%62s%s│%s\n", GRAD_4, COLOR_RESET, "", GRAD_4, COLOR_RESET);
    
    // Divider
    printf("  %s├", GRAD_5);
    for (int i = 0; i < 62; i++) printf("─");
    printf("┤%s\n", COLOR_RESET);
    
    // Version and tagline
    printf("  %s│%s  %s%s v%s%s %s│%s %sAdvanced Scientific Calculator%s              %s│%s\n",
           GRAD_5, COLOR_RESET, 
           COLOR_MINT, SYM_SPARK, VERSION, COLOR_RESET,
           STYLE_DIM, COLOR_RESET,
           COLOR_SKY, COLOR_RESET,
           GRAD_5, COLOR_RESET);
    
    // Empty line
    printf("  %s│%s%62s%s│%s\n", GRAD_5, COLOR_RESET, "", GRAD_5, COLOR_RESET);
    
    // Feature highlights with modern icons
    printf("  %s│%s   %s◆%s 12 Categories   %s◆%s 35+ Commands   %s◆%s 100+ Concepts    %s│%s\n",
           GRAD_6, COLOR_RESET,
           COLOR_TEAL, COLOR_RESET,
           COLOR_PURPLE, COLOR_RESET,
           COLOR_PINK, COLOR_RESET,
           GRAD_6, COLOR_RESET);
    
    // Bottom border
    printf("  %s╰", GRAD_6);
    for (int i = 0; i < 62; i++) printf("─");
    printf("╯%s\n\n", COLOR_RESET);
}

void print_separator(const char *title) {
    printf("\n  %s┌─%s %s%s%s %s", COLOR_TEAL, COLOR_RESET, COLOR_BOLD_WHITE, title, COLOR_RESET, COLOR_TEAL);
    int title_len = (int)strlen(title);
    for (int i = 0; i < 50 - title_len; i++) printf("─");
    printf("%s\n", COLOR_RESET);
}

void print_help(void) {
    printf("\n");
    
    // Header
    printf("  %s╭──────────────────────────────────────────────────────────────────╮%s\n", COLOR_TEAL, COLOR_RESET);
    printf("  %s│%s   %s%s COMMAND REFERENCE%s                                            %s│%s\n",
           COLOR_TEAL, COLOR_RESET, COLOR_GOLD, SYM_SPARK, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
    printf("  %s╰──────────────────────────────────────────────────────────────────╯%s\n", COLOR_TEAL, COLOR_RESET);
    
    // BASIC
    print_separator("BASIC ARITHMETIC");
    printf("    %s▸%s %sbasic%s add|sub|mul|div <a> <b>   %sTwo-number operations%s\n", 
           COLOR_MINT, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sbasic%s mod|pow|percent <a> <b>   %sModulo, power, percentage%s\n",
           COLOR_MINT, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sbasic%s abs|sqrt <value>          %sSingle-value operations%s\n",
           COLOR_MINT, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sbasic%s sum|product|avg <vals...> %sMulti-value operations%s\n",
           COLOR_MINT, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sbasic%s min|max <vals...>         %sFind min/max of many%s\n",
           COLOR_MINT, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("      %s→%s basic sum 1 2 3 4 5 6 7 8 9 10 %s⟹%s %s55%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_TEAL, COLOR_RESET, COLOR_LIME, COLOR_RESET);
    
    // SCIENTIFIC
    print_separator("SCIENTIFIC FUNCTIONS");
    printf("    %s▸%s %sscientific%s sin|cos|tan <x> [deg]  %sTrigonometry%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s asin|acos|atan <x>     %sInverse trig%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s [a]sinh|[a]cosh|[a]tanh %sHyperbolic & Inverse%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s log|log10|log2|exp <x> %sLog & exponential%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s log_base <x> <b>       %sLog arbitrary base%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s rand                   %sRandom [0,1)%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s sqrt|cbrt|nroot <x> [n] %sRoots%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s fact|perm|comb <n> [r]  %sFactorial, nPr, nCr%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s gcd|lcm|isprime <a> [b] %sNumber theory%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sscientific%s floor|ceil|round <x>   %sRounding%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("      %s→%s scientific comb 10 3 %s⟹%s %s120%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_TEAL, COLOR_RESET, COLOR_LIME, COLOR_RESET);
    
    // STATISTICAL
    print_separator("STATISTICAL ANALYSIS");
    printf("    %s▸%s %sstatistical%s mean|median|mode <vals...>    %sCentral tendency%s\n",
           COLOR_PURPLE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sstatistical%s stddev|variance|range <vals>  %sDispersion%s\n",
           COLOR_PURPLE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sstatistical%s quartiles <vals...>           %sQ1, Median, Q3, IQR%s\n",
           COLOR_PURPLE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sstatistical%s correlation|covariance <x...> <y...> %sPaired statistics%s\n",
           COLOR_PURPLE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sstatistical%s regression <x...> <y...>      %sLinear fit y = mx + b%s\n",
           COLOR_PURPLE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sstatistical%s percentile <p> <vals...>      %sPercentile (0-100)%s\n",
           COLOR_PURPLE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("      %s→%s statistical median 1 2 3 4 5 6 7 8 9 %s⟹%s %s5%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_TEAL, COLOR_RESET, COLOR_LIME, COLOR_RESET);
    
    // SOLVER
    print_separator("EQUATION SOLVERS");
    printf("    %s▸%s %ssolver%s det2x2|det3x3 <matrix values> %sMatrix determinants%s\n",
           COLOR_CORAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %ssolver%s trace <n> <n*n matrix values> %sMatrix trace%s\n",
           COLOR_CORAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %ssolver%s quadratic <a> <b> <c>        %sax² + bx + c = 0%s\n",
           COLOR_CORAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %ssolver%s linear2|linear3 <coeffs...>  %sLinear systems%s\n",
           COLOR_CORAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %ssolver%s roots <expr> <a> <b>         %sBisection root finding%s\n",
           COLOR_CORAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %ssolver%s newton <expr> <x0>           %sNewton-Raphson solver%s\n",
           COLOR_CORAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %ssolver%s poly <x> <coeffs...>         %sPolynomial evaluation%s\n",
           COLOR_CORAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("      %s→%s solver quadratic 1 -5 6 %s⟹%s %sx=3, x=2%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_TEAL, COLOR_RESET, COLOR_LIME, COLOR_RESET);
    
    // GRAPHICAL
    print_separator("GRAPHICAL PLOTTING");
    printf("    %s▸%s %sgraphical plot%s <expression>       %sASCII function plotter%s\n",
           COLOR_TEAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("      %sOperators:%s + - * / ^   %sFunctions:%s sin cos tan sqrt log exp\n", 
           COLOR_LAVENDER, COLOR_RESET, COLOR_LAVENDER, COLOR_RESET);
    printf("      %s→%s graphical plot %sx^3 - 3*x%s\n", COLOR_GOLD, COLOR_RESET, COLOR_LIME, COLOR_RESET);
    
    // CALCULUS
    print_separator("CALCULUS");
    printf("    %s▸%s %scalculus integral%s <expr> <a> <b>  %sNumerical integration%s\n",
           COLOR_ORANGE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("      %s→%s calculus integral x^2 0 1 %s⟹%s %s0.333%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_TEAL, COLOR_RESET, COLOR_LIME, COLOR_RESET);
    
    // COMPLEX
    print_separator("COMPLEX NUMBERS");
    printf("    %s▸%s %scomplex%s add|sub|mul|div <z1> <z2>  %sComplex arithmetic%s\n",
           COLOR_PINK, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %scomplex mag%s <a+bi>                 %sMagnitude |z|%s\n",
           COLOR_PINK, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("      %s→%s complex mul 3+4i 1+2i %s⟹%s %s-5+10i%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_TEAL, COLOR_RESET, COLOR_LIME, COLOR_RESET);
    
    // FINANCIAL
    print_separator("FINANCIAL");
    printf("    %s▸%s %sfinancial npv%s <rate> <cf...>       %sNet Present Value%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sfinancial irr%s <cf0> <cf1...>       %sInternal Rate of Return%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sfinancial compound%s <P> <r> <n>     %sCompound interest%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sfinancial pmt%s <r> <n> <pv>         %sLoan payment%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sfinancial fv|pv%s <r> <n> <pmt> <v>  %sFuture/Present Value%s\n",
           COLOR_GOLD, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    
    // MEMORY
    print_separator("MEMORY & CONVERTERS");
    printf("    %s▸%s %smemory%s store|recall|list          %sStore/recall values (BST)%s\n",
           COLOR_MINT, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %shistory%s show|last|clear           %sResult timeline controls (Queue)%s\n",
           COLOR_PURPLE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sprogramming%s concepts|ds_demo      %sShow C concepts & data struct demos%s\n",
           COLOR_SKY, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sconverter%s to_binary|check_bit     %sBit manipulation%s\n",
           COLOR_TEAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sconverter%s length|temp <v> <f> <t> %sUnit conversion%s\n",
           COLOR_TEAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    printf("    %s▸%s %sextra%s save|load|save_bin|load_bin %sFile persistence (Text/Binary)%s\n",
           COLOR_CORAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    
    // SYSTEM
    print_separator("SYSTEM");
    printf("    %s▸%s %shelp%s    %sShow this reference%s    %s▸%s %squit%s    %sExit calculator%s\n",
           STYLE_DIM, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET,
           STYLE_DIM, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, STYLE_DIM, COLOR_RESET);
    
    // Footer tip
    printf("\n  %s╭──────────────────────────────────────────────────────────────────╮%s\n", COLOR_TEAL, COLOR_RESET);
    printf("  %s│%s   %s%s%s Supports large datasets, multi-value operations, and equations %s│%s\n",
           COLOR_TEAL, COLOR_RESET,
           COLOR_MINT, SYM_INFO, COLOR_RESET,
           COLOR_TEAL, COLOR_RESET);
    printf("  %s╰──────────────────────────────────────────────────────────────────╯%s\n\n", COLOR_TEAL, COLOR_RESET);
}

// CONCEPT: Main entry point with CLI loop
int main(void) {
    char input[MAX_CMD_LEN];
    
    setup_signal_handlers();
    initialize_calculator();
    print_banner();
    
    // Quick start hint
    printf("  %s┌─%s %sTip%s\n", COLOR_TEAL, COLOR_RESET, COLOR_MINT, COLOR_RESET);
    printf("  %s│%s  Type %shelp%s for commands, %squit%s to exit\n", 
           COLOR_TEAL, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET, COLOR_BOLD_WHITE, COLOR_RESET);
    printf("  %s│%s  Try: %sbasic sum 1 2 3 4 5%s or %ssolver roots x^2 - 2 0 2%s\n",
           COLOR_TEAL, COLOR_RESET, COLOR_GOLD, COLOR_RESET, COLOR_GOLD, COLOR_RESET);
    printf("  %s└───────────────────────────────────────────────%s\n\n", COLOR_TEAL, COLOR_RESET);

    while (true) {
        if (is_interrupted()) {
            printf("\n\n");
            printf("  %s╭────────────────────────────────╮%s\n", COLOR_CORAL, COLOR_RESET);
            printf("  %s│%s  %s%s%s Interrupt received (Ctrl+C)   %s│%s\n", 
                   COLOR_CORAL, COLOR_RESET, COLOR_GOLD, SYM_WARN, COLOR_RESET, COLOR_CORAL, COLOR_RESET);
            printf("  %s╰────────────────────────────────╯%s\n\n", COLOR_CORAL, COLOR_RESET);
            break;
        }

        // Modern prompt with lambda symbol
        printf("  %s%s%s %s›%s ", COLOR_PURPLE, SYM_LAMBDA, COLOR_RESET, COLOR_MINT, COLOR_RESET);
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (is_interrupted()) {
                printf("\n\n");
                printf("  %s╭────────────────────────────────╮%s\n", COLOR_CORAL, COLOR_RESET);
                printf("  %s│%s  %s%s%s Interrupt received (Ctrl+C)   %s│%s\n", 
                       COLOR_CORAL, COLOR_RESET, COLOR_GOLD, SYM_WARN, COLOR_RESET, COLOR_CORAL, COLOR_RESET);
                printf("  %s╰────────────────────────────────╯%s\n\n", COLOR_CORAL, COLOR_RESET);
            } else {
                printf("\n");
            }
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            printf("\n");
            printf("  %s╭────────────────────────────────╮%s\n", COLOR_TEAL, COLOR_RESET);
            printf("  %s│%s  %s%s%s Thanks for calculating!      %s│%s\n", 
                   COLOR_TEAL, COLOR_RESET, COLOR_MINT, SYM_SPARK, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("  %s│%s  %sCalculator v%s%s                  %s│%s\n",
                   COLOR_TEAL, COLOR_RESET, STYLE_DIM, VERSION, COLOR_RESET, COLOR_TEAL, COLOR_RESET);
            printf("  %s╰────────────────────────────────╯%s\n\n", COLOR_TEAL, COLOR_RESET);
            break;
        } else if (strcmp(input, "help") == 0) {
            print_help();
            continue;
        } else if (strcmp(input, "clear") == 0) {
            printf("\033[2J\033[H");
            print_banner();
            continue;
        } else if (strlen(input) == 0) {
            continue;
        }

        handle_command(input);
    }

    cleanup_calculator();
    return 0;
}
