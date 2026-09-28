#if !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 199309L
#endif
#include "initialization.h"
#include "utils/utils.h"
#include "common.h"
#include <stdio.h>
#include <time.h>

static void print_progress_bar(int step, int total) {
    printf("\r  %s", COLOR_TEAL);
    for (int i = 0; i < total; i++) {
        if (i < step) printf("━");
        else printf("%s━%s", STYLE_DIM, COLOR_TEAL);
    }
    printf("%s ", COLOR_RESET);
    fflush(stdout);
}

static void sleep_milliseconds(long milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

void initialize_calculator(void) {
    printf("\n");
    
    // Quick progress animation
    const char *steps[] = {"Loading modules", "Initializing parser", "Ready"};
    for (int i = 0; i < 3; i++) {
        print_progress_bar(i + 1, 3);
        printf("%s%s%s", STYLE_DIM, steps[i], COLOR_RESET);
        fflush(stdout);
        sleep_milliseconds(80);
    }
    printf("\n\n");
}
