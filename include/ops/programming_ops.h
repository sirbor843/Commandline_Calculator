#ifndef PROGRAMMING_OPS_H
#define PROGRAMMING_OPS_H

#include "common.h"

// CONCEPT: Function pointers for scripting (Day 23)
typedef CalculationResult (*ScriptFunction)(double *, int);

typedef struct {
    const char *name;
    ScriptFunction func;
} ScriptEntry;

void programming_run_script(const char *script_name);
void programming_list_scripts(void);
CalculationResult programming_execute(const char *cmd, double *args, int nargs);
void programming_demo_ds(const char *type);

#endif // PROGRAMMING_OPS_H
