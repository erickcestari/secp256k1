#include <stddef.h>
#include <stdint.h>

/*** Field Elements ***/
void fuzz_fe_add_commutativity(const uint8_t *data, size_t size);
void fuzz_fe_mul_commutativity(const uint8_t *data, size_t size);
void fuzz_fe_mul_associativity(const uint8_t *data, size_t size);
void fuzz_fe_sqr_vs_mul(const uint8_t *data, size_t size);
void fuzz_fe_neg_add_zero(const uint8_t *data, size_t size);
void fuzz_fe_inv_consistency(const uint8_t *data, size_t size);
void fuzz_fe_sqrt_roundtrip(const uint8_t *data, size_t size);
void fuzz_fe_normalize_idempotent(const uint8_t *data, size_t size);
void fuzz_fe_b32_roundtrip(const uint8_t *data, size_t size);
void fuzz_fe_get_bits_parity(const uint8_t *data, size_t size);