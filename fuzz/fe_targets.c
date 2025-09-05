#include <assert.h>
#include "src/util.h"
#include "src/int128.h"
#include "src/int128_impl.h"
#include "src/field.h"
#include "src/field_impl.h"
#include "fe_targets.h"

/* === Helpers === */

/* Total mapping: 32 bytes -> fe (mod p). Sets metadata for VERIFY. */
static void fe_from_bytes_mod(secp256k1_fe* r, const uint8_t* b32) {
    secp256k1_fe_impl_set_b32_mod(r, b32);
#ifdef VERIFY
    r->magnitude = 1;
    r->normalized = 0;
#endif
    /* Optional: many ops accept non-normalized inputs, but normalize for
       predictable equality comparisons. */
    secp256k1_fe_normalize(r);
}

/* Strict parse (< p). Returns 1 on success, 0 on reject. */
static int fe_from_bytes_strict(secp256k1_fe* r, const uint8_t* b32) {
    int ok = secp256k1_fe_impl_set_b32_limit(r, b32);
#ifdef VERIFY
    if (ok) { r->magnitude = 1; r->normalized = 0; }
#endif
    return ok;
}

/* Compare (expects normalized inputs). */
static int fe_equal_norm(const secp256k1_fe* a, const secp256k1_fe* b) {
    return secp256k1_fe_equal(a, b);
}

/* === Targets === */

/* a + b == b + a */
void fuzz_fe_add_commutativity(const uint8_t *data, size_t size) {
    if (size < 64) return;
    secp256k1_fe a,b,r1,r2;
    fe_from_bytes_mod(&a, data);
    fe_from_bytes_mod(&b, data+32);

    r1 = a; secp256k1_fe_add(&r1, &b);  secp256k1_fe_normalize(&r1);
    r2 = b; secp256k1_fe_add(&r2, &a);  secp256k1_fe_normalize(&r2);

    CHECK(fe_equal_norm(&r1, &r2));
}

/* a * b == b * a */
void fuzz_fe_mul_commutativity(const uint8_t *data, size_t size) {
    if (size < 64) return;
    secp256k1_fe a,b,r1,r2;
    fe_from_bytes_mod(&a, data);
    fe_from_bytes_mod(&b, data+32);

    secp256k1_fe_mul(&r1, &a, &b); secp256k1_fe_normalize(&r1);
    secp256k1_fe_mul(&r2, &b, &a); secp256k1_fe_normalize(&r2);

    CHECK(fe_equal_norm(&r1, &r2));
}

/* (a*b)*c == a*(b*c) */
void fuzz_fe_mul_associativity(const uint8_t *data, size_t size) {
    if (size < 96) return;
    secp256k1_fe a,b,c,t1,t2,r1,r2;
    fe_from_bytes_mod(&a, data);
    fe_from_bytes_mod(&b, data+32);
    fe_from_bytes_mod(&c, data+64);

    secp256k1_fe_mul(&t1, &a, &b);
    secp256k1_fe_mul(&r1, &t1, &c); secp256k1_fe_normalize(&r1);

    secp256k1_fe_mul(&t2, &b, &c);
    secp256k1_fe_mul(&r2, &a, &t2); secp256k1_fe_normalize(&r2);

    CHECK(fe_equal_norm(&r1, &r2));
}

/* sqr(a) == a * a */
void fuzz_fe_sqr_vs_mul(const uint8_t *data, size_t size) {
    if (size < 32) return;
    secp256k1_fe a,r1,r2;
    fe_from_bytes_mod(&a, data);

    secp256k1_fe_sqr(&r1, &a);     secp256k1_fe_normalize(&r1);
    secp256k1_fe_mul(&r2, &a, &a); secp256k1_fe_normalize(&r2);

    CHECK(fe_equal_norm(&r1, &r2));
}

/* a + (-a) == 0 */
void fuzz_fe_neg_add_zero(const uint8_t *data, size_t size) {
    if (size < 32) return;
    secp256k1_fe a,neg,sum;
    fe_from_bytes_mod(&a, data);

    /* Input is normalized → magnitude(a)=1, so m=1 is fine. */
    secp256k1_fe_negate(&neg, &a, 1);
    sum = a; secp256k1_fe_add(&sum, &neg); secp256k1_fe_normalize(&sum);

    CHECK(secp256k1_fe_is_zero(&sum));
}

/* inv(a) * a == 1  (for a != 0); for a == 0 -> inv(a)==0 */
void fuzz_fe_inv_consistency(const uint8_t *data, size_t size) {
    if (size < 32) return;
    secp256k1_fe a,inv,prod,one;
    fe_from_bytes_mod(&a, data);

    if (secp256k1_fe_is_zero(&a)) {
        secp256k1_fe_inv(&inv, &a); secp256k1_fe_normalize(&inv);
        CHECK(secp256k1_fe_is_zero(&inv));
        return;
    }
    secp256k1_fe_inv(&inv, &a);         secp256k1_fe_normalize(&inv);
    secp256k1_fe_mul(&prod, &a, &inv);  secp256k1_fe_normalize(&prod);

    secp256k1_fe_set_int(&one, 1);      secp256k1_fe_normalize(&one);
    CHECK(fe_equal_norm(&prod, &one));
}

/* sqrt roundtrip: x = a^2 is a QR → sqrt(x) must succeed and square back */
void fuzz_fe_sqrt_roundtrip(const uint8_t *data, size_t size) {
    if (size < 32) return;
    secp256k1_fe x,r,rr,a;
    fe_from_bytes_mod(&a, data);

    secp256k1_fe_sqr(&x, &a);   secp256k1_fe_normalize(&x);
    int ok = secp256k1_fe_sqrt(&r, &x);
    CHECK(ok == 1);

    secp256k1_fe_sqr(&rr, &r);  secp256k1_fe_normalize(&rr);
    secp256k1_fe_normalize(&x);

    CHECK(fe_equal_norm(&rr, &x));
}

/* normalize(normalize(a)) == normalize(a) */
void fuzz_fe_normalize_idempotent(const uint8_t *data, size_t size) {
    if (size < 32) return;
    secp256k1_fe a,b,c;

    if (!fe_from_bytes_strict(&a, data)) return; /* reject >= p */
#ifdef VERIFY
    /* a: magnitude=1, normalized=0 (set by helper) */
#endif
    b = a; secp256k1_fe_normalize(&b);
    c = b; secp256k1_fe_normalize(&c);

    CHECK(fe_equal_norm(&b, &c));
}

/* get_b32(set_b32(x)) == canonical x  (round-trip) */
void fuzz_fe_b32_roundtrip(const uint8_t *data, size_t size) {
    if (size < 32) return;
    secp256k1_fe a,b;
    unsigned char buf[32];

    fe_from_bytes_mod(&a, data);       /* a normalized */
    secp256k1_fe_get_b32(buf, &a);

    if (!fe_from_bytes_strict(&b, buf)) return; /* serialized is canonical */
    secp256k1_fe_normalize(&b);

    CHECK(fe_equal_norm(&a, &b));
}

/* fe_is_odd parity matches the lsb of get_b32 */
void fuzz_fe_get_bits_parity(const uint8_t *data, size_t size) {
    if (size < 32) return;
    secp256k1_fe a;
    unsigned char buf[32];
    fe_from_bytes_mod(&a, data);

    int odd = secp256k1_fe_is_odd(&a);
    secp256k1_fe_get_b32(buf, &a);
    int lsb = buf[31] & 1;

    CHECK(odd == lsb);
}
