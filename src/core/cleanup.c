#include "cleanup.h"
#include "common.h"
#include "ops/history_ops.h"
#include "ops/memory_ops.h"
#include "utils/utils.h"
#include <stdio.h>

void cleanup_calculator(void) {
  memory_cleanup();
  history_cleanup();
  printf("  %s%s%s Resources freed. Session ended.%s\n\n", STYLE_DIM, SYM_CHECK,
         COLOR_RESET, COLOR_RESET);
}
