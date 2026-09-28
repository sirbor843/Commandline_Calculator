#include "utils/signals.h"
#include <signal.h>

// CONCEPT: sig_atomic_t for lock-free, async-signal-safe state signaling
static volatile sig_atomic_t g_interrupted = 0;

static void handle_sigint(int sig) {
    (void)sig;
    g_interrupted = 1;
}

void setup_signal_handlers(void) {
    signal(SIGINT, handle_sigint);
}

bool check_and_clear_interrupted(void) {
    if (g_interrupted) {
        g_interrupted = 0;
        return true;
    }
    return false;
}

bool is_interrupted(void) {
    return g_interrupted != 0;
}
