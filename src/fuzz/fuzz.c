#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "scalar_targets.c"
#include "targets.h"

typedef void (*fuzz_function)(const uint8_t* data, size_t size);
static fuzz_function selected_fuzz_function = NULL;

int LLVMFuzzerInitialize(int *argc, char ***argv) {
    const char* fuzz_target = getenv("FUZZ");
    if (!fuzz_target) {
        fprintf(stderr, "Select a fuzz target using the FUZZ environment variable\n");
        assert(0);
    }
    if (strcmp(fuzz_target, "scalar_add_commutativty") == 0) {
        selected_fuzz_function = &fuzz_scalar_add_commutativty;
    } else if (strcmp(fuzz_target, "scalar_add_associativity") == 0) {
        selected_fuzz_function = &fuzz_scalar_add_associativity;
    } else if (strcmp(fuzz_target, "scalar_add_zero") == 0) {
        selected_fuzz_function = &fuzz_scalar_add_zero;
    } else if (strcmp(fuzz_target, "scalar_add_complements") == 0) {
        selected_fuzz_function = &fuzz_scalar_add_complements;
    } else if (strcmp(fuzz_target, "scalar_mul_commutativity") == 0) {
        selected_fuzz_function = &fuzz_scalar_mul_commutativity;
    } else if (strcmp(fuzz_target, "scalar_mul_associativity") == 0) {
        selected_fuzz_function = &fuzz_scalar_mul_associativity;
    } else if (strcmp(fuzz_target, "scalar_mul_distributivity") == 0) {
        selected_fuzz_function = &fuzz_scalar_mul_distributivity;
    } else if (strcmp(fuzz_target, "scalar_mul_one") == 0) {
        selected_fuzz_function = &fuzz_scalar_mul_one;
    } else if (strcmp(fuzz_target, "scalar_mul_zero") == 0) {
        selected_fuzz_function = &fuzz_scalar_mul_zero;
    } else if (strcmp(fuzz_target, "scalar_inverse") == 0) {
        selected_fuzz_function = &fuzz_scalar_inverse;
    } else if (strcmp(fuzz_target, "scalar_inverse_var") == 0) {
        selected_fuzz_function = &fuzz_scalar_inverse_var;
    } else if (strcmp(fuzz_target, "scalar_negate") == 0) {
        selected_fuzz_function = &fuzz_scalar_negate;
    } else if (strcmp(fuzz_target, "scalar_shift") == 0) {
        selected_fuzz_function = &fuzz_scalar_shift;
    } else if (strcmp(fuzz_target, "scalar_split_lambda") == 0) {
        selected_fuzz_function = &fuzz_scalar_split_lambda;
    } else if (strcmp(fuzz_target, "scalar_cmov") == 0) {
        selected_fuzz_function = &fuzz_scalar_cmov;
    } else {
        fprintf(stderr, "Unknown fuzz target selected using FUZZ environment variable: %s\n", fuzz_target);
        assert(0);
    }
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    selected_fuzz_function(data, size);
    return 0;
}