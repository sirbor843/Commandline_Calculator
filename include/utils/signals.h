#ifndef SIGNALS_H
#define SIGNALS_H

#include <stdbool.h>

// CONCEPT: Signal handling with async-signal-safe flag (Day 30)
void setup_signal_handlers(void);
bool check_and_clear_interrupted(void);
bool is_interrupted(void);

#endif // SIGNALS_H
