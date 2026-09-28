#include "ops/converter_ops.h"
#include <string.h>

// CONCEPT: Bitwise operations to extract binary digits (Day 24)
void converter_format_binary(int n, char *out, size_t max_len) {
    if (!out || max_len == 0) return;
    unsigned int un = (unsigned int)n;
    char buffer[33];
    buffer[32] = '\0';
    for (int i = 31; i >= 0; i--) {
        buffer[31 - i] = ((un >> i) & 1) ? '1' : '0';
    }
    
    // Find first '1' or keep at least single digit '0'
    const char *p = buffer;
    while (*p == '0' && *(p + 1) != '\0') {
        p++;
    }
    strncpy(out, p, max_len - 1);
    out[max_len - 1] = '\0';
}

CalculationResult converter_to_binary(int n) {
    CalculationResult res;
    res.is_error = false;
    strcpy(res.err_message, "");
    char bin_str[64];
    converter_format_binary(n, bin_str, sizeof(bin_str));
    res.value = strtod(bin_str, NULL);
    return res;
}

// CONCEPT: Right shift and mask (Day 24)
CalculationResult converter_check_bit(int n, int bit) {
    CalculationResult res;
    if (bit < 0 || bit > 31) {
        res.value = 0;
        res.is_error = true;
        strcpy(res.err_message, "Bit index must be between 0 and 31.");
    } else {
        // CONCEPT: (n >> bit) & 1
        res.value = (double)((n >> bit) & 1);
        res.is_error = false;
        strcpy(res.err_message, "");
    }
    return res;
}

// CONCEPT: Lookup table for length conversion factors (to meters)
typedef struct {
    const char *unit;
    double to_meter;
} LengthUnit;

static LengthUnit length_table[] = {
    {"m", 1.0},
    {"cm", 0.01},
    {"mm", 0.001},
    {"km", 1000.0},
    {"in", 0.0254},
    {"ft", 0.3048},
    {"yd", 0.9144},
    {"mi", 1609.344},
    {NULL, 0.0}
};

// CONCEPT: Table lookup for unit conversion
static double get_length_factor(const char *unit) {
    for (int i = 0; length_table[i].unit != NULL; i++) {
        if (strcmp(length_table[i].unit, unit) == 0) {
            return length_table[i].to_meter;
        }
    }
    return -1.0; // Not found
}

CalculationResult converter_length(double value, const char *from, const char *to) {
    CalculationResult res = {0.0, false, ""};
    
    double from_factor = get_length_factor(from);
    double to_factor = get_length_factor(to);
    
    if (from_factor < 0 || to_factor < 0) {
        res.is_error = true;
        strcpy(res.err_message, "Unknown unit. Use: m, cm, mm, km, in, ft, yd, mi");
        return res;
    }
    
    // Convert: value * from_factor gives meters, then divide by to_factor
    res.value = value * from_factor / to_factor;
    return res;
}

// CONCEPT: Temperature conversion (non-linear)
CalculationResult converter_temperature(double value, const char *from, const char *to) {
    CalculationResult res = {0.0, false, ""};
    double celsius;
    
    // Convert to Celsius first
    if (strcmp(from, "C") == 0) {
        celsius = value;
    } else if (strcmp(from, "F") == 0) {
        celsius = (value - 32.0) * 5.0 / 9.0;
    } else if (strcmp(from, "K") == 0) {
        celsius = value - 273.15;
    } else {
        res.is_error = true;
        strcpy(res.err_message, "Unknown unit. Use: C, F, K");
        return res;
    }
    
    // Convert from Celsius to target
    if (strcmp(to, "C") == 0) {
        res.value = celsius;
    } else if (strcmp(to, "F") == 0) {
        res.value = celsius * 9.0 / 5.0 + 32.0;
    } else if (strcmp(to, "K") == 0) {
        res.value = celsius + 273.15;
    } else {
        res.is_error = true;
        strcpy(res.err_message, "Unknown unit. Use: C, F, K");
    }
    
    return res;
}
