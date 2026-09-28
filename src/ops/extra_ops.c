#include "ops/extra_ops.h"
#include <stdio.h>
#include <string.h>

// CONCEPT: fopen, fprintf, fclose (Day 18)
CalculationResult extra_save_result(double val, const char *filename) {
    CalculationResult res = {0.0, false, ""};
    
    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        res.is_error = true;
        strcpy(res.err_message, "Could not open file for writing.");
        return res;
    }
    
    fprintf(f, "Result: %.2f\n", val);
    fclose(f);
    
    res.value = val;
    return res;
}

// CONCEPT: fwrite - binary file output (Day 19)
CalculationResult extra_save_binary(double *data, int n, const char *filename) {
    CalculationResult res = {0.0, false, ""};
    
    FILE *f = fopen(filename, "wb"); // CONCEPT: "wb" for write binary
    if (f == NULL) {
        res.is_error = true;
        strcpy(res.err_message, "Could not open file for binary writing.");
        return res;
    }
    
    // CONCEPT: fwrite(ptr, element_size, count, stream)
    size_t written = fwrite(data, sizeof(double), n, f);
    
    // CONCEPT: File error handling (Day 21)
    if (ferror(f)) {
        res.is_error = true;
        strcpy(res.err_message, "Write error occurred.");
        fclose(f);
        return res;
    }
    
    fclose(f);
    res.value = (double)written;
    return res;
}

// CONCEPT: fread - binary file input (Day 19)
CalculationResult extra_load_binary(double *data, int max_n, const char *filename, int *out_n) {
    CalculationResult res = {0.0, false, ""};
    *out_n = 0;
    
    FILE *f = fopen(filename, "rb"); // CONCEPT: "rb" for read binary
    if (f == NULL) {
        res.is_error = true;
        strcpy(res.err_message, "Could not open file for binary reading.");
        return res;
    }
    
    // CONCEPT: fread(ptr, element_size, count, stream)
    size_t read_count = fread(data, sizeof(double), max_n, f);
    
    // CONCEPT: feof and ferror for robust I/O (Day 21)
    if (ferror(f)) {
        res.is_error = true;
        strcpy(res.err_message, "Read error occurred.");
        fclose(f);
        return res;
    }
    
    *out_n = (int)read_count;
    fclose(f);
    res.value = (double)read_count;
    return res;
}

// CONCEPT: Robust text file reading with error handling (Day 21)
CalculationResult extra_load_data(const char *filename) {
    CalculationResult res = {0.0, false, ""};
    
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        res.is_error = true;
        strcpy(res.err_message, "Could not open file for reading.");
        return res;
    }
    
    double sum = 0.0;
    int count = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), f)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (strncmp(p, "Result:", 7) == 0) {
            p += 7;
            while (*p == ' ' || *p == '\t') p++;
        }
        double val;
        if (sscanf(p, "%lf", &val) == 1) {
            sum += val;
            count++;
        }
    }
    
    if (ferror(f)) {
        res.is_error = true;
        strcpy(res.err_message, "Error reading file.");
        fclose(f);
        return res;
    }
    
    fclose(f);
    if (count == 0) {
        res.is_error = true;
        strcpy(res.err_message, "No valid numbers found in file.");
        return res;
    }
    res.value = sum / count; // Return average
    return res;
}
