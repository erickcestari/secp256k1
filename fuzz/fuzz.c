#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "fuzz/scalar_targets.h"
#include "fuzz/fe_targets.h"

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
    } else if (strcmp(fuzz_target, "scalar_half_double") == 0) {
        selected_fuzz_function = &fuzz_scalar_half_double;
    } else if (strcmp(fuzz_target, "scalar_inverse_consistency") == 0) {
        selected_fuzz_function = &fuzz_scalar_inverse_consistency;
    } else if (strcmp(fuzz_target, "scalar_cond_negate_contract") == 0) {
        selected_fuzz_function = &fuzz_scalar_cond_negate_contract;
    } else if (strcmp(fuzz_target, "scalar_high_normalization") == 0) {
        selected_fuzz_function = &fuzz_scalar_high_normalization;
    } else if (strcmp(fuzz_target, "scalar_b32_roundtrip") == 0) {
        selected_fuzz_function = &fuzz_scalar_b32_roundtrip;
    } else if (strcmp(fuzz_target, "scalar_signed62_roundtrip") == 0) {
        selected_fuzz_function = &fuzz_scalar_signed62_roundtrip;
    } else if (strcmp(fuzz_target, "scalar_square_via_mul") == 0) {
        selected_fuzz_function = &fuzz_scalar_square_via_mul;
    } else if (strcmp(fuzz_target, "scalar_mul_distributivity_right") == 0) {
        selected_fuzz_function = &fuzz_scalar_mul_distributivity_right;
    } else if (strcmp(fuzz_target, "scalar_get_bits_consistency") == 0) {
        selected_fuzz_function = &fuzz_scalar_get_bits_consistency;
    } else if (strcmp(fuzz_target, "fe_add_commutativity") == 0) {
    selected_fuzz_function = &fuzz_fe_add_commutativity;
    } else if (strcmp(fuzz_target, "fe_mul_commutativity") == 0) {
        selected_fuzz_function = &fuzz_fe_mul_commutativity;
    } else if (strcmp(fuzz_target, "fe_mul_associativity") == 0) {
        selected_fuzz_function = &fuzz_fe_mul_associativity;
    } else if (strcmp(fuzz_target, "fe_sqr_vs_mul") == 0) {
        selected_fuzz_function = &fuzz_fe_sqr_vs_mul;
    } else if (strcmp(fuzz_target, "fe_neg_add_zero") == 0) {
        selected_fuzz_function = &fuzz_fe_neg_add_zero;
    } else if (strcmp(fuzz_target, "fe_inv_consistency") == 0) {
        selected_fuzz_function = &fuzz_fe_inv_consistency;
    } else if (strcmp(fuzz_target, "fe_sqrt_roundtrip") == 0) {
        selected_fuzz_function = &fuzz_fe_sqrt_roundtrip;
    } else if (strcmp(fuzz_target, "fe_normalize_idempotent") == 0) {
        selected_fuzz_function = &fuzz_fe_normalize_idempotent;
    } else if (strcmp(fuzz_target, "fe_b32_roundtrip") == 0) {
        selected_fuzz_function = &fuzz_fe_b32_roundtrip;
    } else if (strcmp(fuzz_target, "fe_get_bits_parity") == 0) {
        selected_fuzz_function = &fuzz_fe_get_bits_parity;
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
