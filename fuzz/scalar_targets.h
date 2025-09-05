#include <stddef.h>
#include <stdint.h>

/*** Scalar ***/
void fuzz_scalar_add_commutativty(const uint8_t *data, size_t size);
void fuzz_scalar_add_associativity(const uint8_t *data, size_t size);
void fuzz_scalar_add_zero(const uint8_t *data, size_t size);
void fuzz_scalar_add_complements(const uint8_t *data, size_t size);
void fuzz_scalar_mul_commutativity(const uint8_t *data, size_t size);
void fuzz_scalar_mul_associativity(const uint8_t *data, size_t size);
void fuzz_scalar_mul_distributivity(const uint8_t *data, size_t size);
void fuzz_scalar_mul_one(const uint8_t *data, size_t size);
void fuzz_scalar_mul_zero(const uint8_t *data, size_t size);
void fuzz_scalar_inverse(const uint8_t *data, size_t size);
void fuzz_scalar_inverse_var(const uint8_t *data, size_t size);
void fuzz_scalar_negate(const uint8_t *data, size_t size);
void fuzz_scalar_shift(const uint8_t *data, size_t size);
void fuzz_scalar_split_lambda(const uint8_t *data, size_t size);
void fuzz_scalar_cmov(const uint8_t *data, size_t size);
void fuzz_scalar_half_double(const uint8_t *data, size_t size);
void fuzz_scalar_inverse_consistency(const uint8_t *data, size_t size);
void fuzz_scalar_cond_negate_contract(const uint8_t *data, size_t size);
void fuzz_scalar_high_normalization(const uint8_t *data, size_t size);
void fuzz_scalar_b32_roundtrip(const uint8_t *data, size_t size);
void fuzz_scalar_signed62_roundtrip(const uint8_t *data, size_t size);
void fuzz_scalar_square_via_mul(const uint8_t *data, size_t size);
void fuzz_scalar_mul_distributivity_right(const uint8_t *data, size_t size);
void fuzz_scalar_get_bits_consistency(const uint8_t *data, size_t size);
