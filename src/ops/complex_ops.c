#include "ops/complex_ops.h"
#include "utils/utils.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// CONCEPT: Struct member access with dot operator (Day 15)
Complex complex_add(Complex a, Complex b) {
    Complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

Complex complex_sub(Complex a, Complex b) {
    Complex result;
    result.real = a.real - b.real;
    result.imag = a.imag - b.imag;
    return result;
}

// CONCEPT: Complex multiplication (a+bi)(c+di) = (ac-bd) + (ad+bc)i
Complex complex_mul(Complex a, Complex b) {
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

// CONCEPT: Complex division with conjugate
Complex complex_div(Complex a, Complex b) {
    Complex result;
    double denom = b.real * b.real + b.imag * b.imag;
    if (denom == 0.0) {
        result.real = NAN;
        result.imag = NAN;
    } else {
        result.real = (a.real * b.real + a.imag * b.imag) / denom;
        result.imag = (a.imag * b.real - a.real * b.imag) / denom;
    }
    return result;
}

double complex_magnitude(Complex z) {
    return sqrt(z.real * z.real + z.imag * z.imag);
}

// CONCEPT: String parsing for complex format "a+bi", "a-bi", "bi", "i", or "a"
CalculationResult complex_parse(const char *str, Complex *out) {
    CalculationResult res = {0.0, false, ""};
    out->real = 0.0;
    out->imag = 0.0;
    
    if (str == NULL || *str == '\0') {
        res.is_error = true;
        strcpy(res.err_message, "Empty complex string.");
        return res;
    }

    if (strcmp(str, "i") == 0 || strcmp(str, "+i") == 0) {
        out->real = 0.0;
        out->imag = 1.0;
        return res;
    }
    if (strcmp(str, "-i") == 0) {
        out->real = 0.0;
        out->imag = -1.0;
        return res;
    }
    
    double real = 0.0, imag = 0.0;
    char sign = '+';
    int matched = 0;
    
    // Try format: a+bi or a-bi
    matched = sscanf(str, "%lf%c%lfi", &real, &sign, &imag);
    if (matched == 3 && (sign == '+' || sign == '-')) {
        out->real = real;
        out->imag = (sign == '-') ? -imag : imag;
        return res;
    }
    
    // Try pure imaginary: bi
    matched = sscanf(str, "%lfi", &imag);
    if (matched == 1) {
        out->imag = imag;
        return res;
    }
    
    // Try pure real: a
    matched = sscanf(str, "%lf", &real);
    if (matched == 1) {
        out->real = real;
        return res;
    }
    
    res.is_error = true;
    strcpy(res.err_message, "Invalid complex format. Use: a+bi");
    return res;
}

void complex_print(Complex z) {
    printf("\n");
    printf("    %s╭────────────────────────╮%s\n", COLOR_PINK, COLOR_RESET);
    if (z.imag >= 0) {
        printf("    %s│%s  %s≈%s  %s%.4g + %.4gi%s    %s│%s\n", 
               COLOR_PINK, COLOR_RESET, COLOR_MINT, COLOR_RESET,
               COLOR_GOLD, z.real, z.imag, COLOR_RESET, COLOR_PINK, COLOR_RESET);
    } else {
        printf("    %s│%s  %s≈%s  %s%.4g - %.4gi%s    %s│%s\n", 
               COLOR_PINK, COLOR_RESET, COLOR_MINT, COLOR_RESET,
               COLOR_GOLD, z.real, -z.imag, COLOR_RESET, COLOR_PINK, COLOR_RESET);
    }
    printf("    %s╰────────────────────────╯%s\n\n", COLOR_PINK, COLOR_RESET);
}
