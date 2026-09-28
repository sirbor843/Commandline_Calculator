#ifndef CONVERTER_OPS_H
#define CONVERTER_OPS_H

#include "common.h"

// CONCEPT: Bit manipulation (Day 24)
CalculationResult converter_to_binary(int n);
CalculationResult converter_check_bit(int n, int bit);
void converter_format_binary(int n, char *out, size_t max_len);

// CONCEPT: Lookup tables for unit conversion
CalculationResult converter_length(double value, const char *from, const char *to);
CalculationResult converter_temperature(double value, const char *from, const char *to);

#endif // CONVERTER_OPS_H
