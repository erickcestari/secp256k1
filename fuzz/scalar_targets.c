#include <assert.h>
#include "src/int128.h"
#include "src/int128_impl.h"
#include "src/scalar.h"
#include "src/scalar_impl.h"
#include "scalar_targets.h"

/*** Scalar Operation ***/
/* Test commutativity of scalar addition */ 
void fuzz_scalar_add_commutativty(const uint8_t *data, size_t size) {
    if (size >= 64) {        
        secp256k1_scalar a, b, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_set_b32(&b, data + 32, NULL);
        secp256k1_scalar_add(&r1, &a, &b);
        secp256k1_scalar_add(&r2, &b, &a);
        CHECK(secp256k1_scalar_eq(&r1, &r2));
    }
}

/* Test associativity of scalar addition */
void fuzz_scalar_add_associativity(const uint8_t *data, size_t size) {
    if (size >= 96) {     
        secp256k1_scalar a, b, c, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_set_b32(&b, data + 32, NULL);
        secp256k1_scalar_set_b32(&c, data + 64, NULL);       
        secp256k1_scalar_add(&r1, &a, &b);
        secp256k1_scalar_add(&r1, &r1, &c);
        secp256k1_scalar_add(&r2, &b, &c);
        secp256k1_scalar_add(&r2, &r2, &a);
        CHECK(secp256k1_scalar_eq(&r1, &r2));
    }
}

/* Test identity addition */ 
void fuzz_scalar_add_zero(const uint8_t *data, size_t size) {
    if (size >= 32) {        
        secp256k1_scalar a, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_add(&r1, &a, &secp256k1_scalar_zero);
        CHECK(secp256k1_scalar_eq(&r1, &a));
        secp256k1_scalar_add(&r2, &secp256k1_scalar_zero, &a);
        CHECK(secp256k1_scalar_eq(&r2, &a));
    }
}

/* Test scalar addition with its complement */ 
void fuzz_scalar_add_complements(const uint8_t *data, size_t size) {
    if (size >= 32) {        
        secp256k1_scalar a, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_negate(&r1, &a);
        secp256k1_scalar_add(&r2, &a, &r1);
        CHECK(secp256k1_scalar_is_zero(&r2));
    }
}

/* Test commutativity of scalar multiplication */
void fuzz_scalar_mul_commutativity(const uint8_t *data, size_t size) {
    if (size >= 64) {        
        secp256k1_scalar a, b, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_set_b32(&b, data + 32, NULL);
        secp256k1_scalar_mul(&r1, &a, &b);
        secp256k1_scalar_mul(&r2, &b, &a);
        CHECK(secp256k1_scalar_eq(&r1, &r2));
    }
}

/* Test associativity of scalar multiplication */
void fuzz_scalar_mul_associativity(const uint8_t *data, size_t size) {
    if (size >= 96) {     
        secp256k1_scalar a, b, c, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_set_b32(&b, data + 32, NULL);
        secp256k1_scalar_set_b32(&c, data + 64, NULL);       
        secp256k1_scalar_mul(&r1, &a, &b);
        secp256k1_scalar_mul(&r1, &r1, &c);
        secp256k1_scalar_mul(&r2, &b, &c);
        secp256k1_scalar_mul(&r2, &r2, &a);
        CHECK(secp256k1_scalar_eq(&r1, &r2));
    }
}

/* Test distributivity of scalar multiplication */
void fuzz_scalar_mul_distributivity(const uint8_t *data, size_t size) {
    if (size >= 96) {     
        secp256k1_scalar a, b, c, r1, r2, r3;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_set_b32(&b, data + 32, NULL);
        secp256k1_scalar_set_b32(&c, data + 64, NULL);       
        secp256k1_scalar_add(&r1, &a, &b);
        secp256k1_scalar_mul(&r1, &r1, &c);
        secp256k1_scalar_mul(&r2, &a, &c);
        secp256k1_scalar_mul(&r3, &b, &c);
        secp256k1_scalar_add(&r2, &r2, &r3);
        CHECK(secp256k1_scalar_eq(&r1, &r2));
    }
}

/* Test identity multiplication */ 
void fuzz_scalar_mul_one(const uint8_t *data, size_t size) {
    if (size >= 32) {        
        secp256k1_scalar a, r1;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_mul(&r1, &a, &secp256k1_scalar_one);
        CHECK(secp256k1_scalar_eq(&r1, &a));
    }
}

/* Test scalar multiplication with zero */ 
void fuzz_scalar_mul_zero(const uint8_t *data, size_t size) {
    if (size >= 32) {        
        secp256k1_scalar a, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_mul(&r1, &a, &secp256k1_scalar_zero);
        CHECK(secp256k1_scalar_is_zero(&r1));
        secp256k1_scalar_mul(&r2, &secp256k1_scalar_zero, &a);
        CHECK(secp256k1_scalar_is_zero(&r2));
    }
}

/* Test scalar inverse */
void fuzz_scalar_inverse(const uint8_t *data, size_t size) {
    if (size >= 32) {     
        secp256k1_scalar a, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_inverse(&r1, &a);
        if (secp256k1_scalar_is_zero(&a)) {
            CHECK(secp256k1_scalar_is_zero(&r1));
        } else {
            secp256k1_scalar_mul(&r2, &a, &r1);
            CHECK(secp256k1_scalar_is_one(&r2));
        }
    }
} 

/* Test scalar inverse (without constant-time guarantee) */
void fuzz_scalar_inverse_var(const uint8_t *data, size_t size) {
    if (size >= 32) {     
        secp256k1_scalar a, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_inverse_var(&r1, &a);
        if (secp256k1_scalar_is_zero(&a)) {
            CHECK(secp256k1_scalar_is_zero(&r1));
        } else {
            secp256k1_scalar_mul(&r2, &a, &r1);
            CHECK(secp256k1_scalar_is_one(&r2));
        }
    }
}             

/* Test scalar complement */ 
void fuzz_scalar_negate(const uint8_t *data, size_t size) {
    if (size >= 32) {        
        secp256k1_scalar a, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_negate(&r1, &a);
        secp256k1_scalar_negate(&r2, &r1);
        CHECK(secp256k1_scalar_eq(&a, &r2));
    }
}

/* Test low bits shifted off */
void fuzz_scalar_shift(const uint8_t *data, size_t size) {
    if (size >= 32) {
        int bit, r1, r2;
        secp256k1_scalar a;
        secp256k1_scalar_set_b32(&a, data, NULL);
        bit = 1 + (data[31] % 15);
        r1 = (int)secp256k1_scalar_get_bits_var(&a, 0, (unsigned)bit);
        r2 = (int)(a.d[0] & ((1ULL << bit) - 1ULL));
        CHECK(r1 == r2);
    }
}

/* Test r1+r2*lambda = a */
void fuzz_scalar_split_lambda(const uint8_t *data, size_t size) {
    if (size >= 32) {
        secp256k1_scalar a, r1, r2, r3;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_split_lambda(&r1, &r2, &a);
        secp256k1_scalar_mul(&r3, &secp256k1_const_lambda, &r2);
        secp256k1_scalar_add(&r3, &r3, &r1);
        CHECK(secp256k1_scalar_eq(&r3, &a));
    }    
}

/* Test conditional move of scalars  */
void fuzz_scalar_cmov(const uint8_t *data, size_t size) {
    if (size >= 64) {
        secp256k1_scalar a, r, orig;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_set_b32(&r, data + 32, NULL);
        orig = r;

        secp256k1_scalar_cmov(&r, &a, 0);
        CHECK(secp256k1_scalar_eq(&r, &orig));

        secp256k1_scalar_cmov(&r, &a, 1);
        CHECK(secp256k1_scalar_eq(&r, &a));
    }
}

/* 2 * half(a) == a  (mod n) */
void fuzz_scalar_half_double(const uint8_t *data, size_t size) {
    if (size >= 32) {
        secp256k1_scalar a, h, twoh;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_half(&h, &a);
        secp256k1_scalar_add(&twoh, &h, &h);
        CHECK(secp256k1_scalar_eq(&twoh, &a));
    }
}

/* inverse == inverse_var (bitwise equal) */
void fuzz_scalar_inverse_consistency(const uint8_t *data, size_t size) {
    if (size >= 32) {
        secp256k1_scalar a, i1, i2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_inverse(&i1, &a);
        secp256k1_scalar_inverse_var(&i2, &a);
        CHECK(secp256k1_scalar_eq(&i1, &i2));
    }
}

/* cond_negate returns 1 when flag=0, -1 when flag=1; and applies negate on flag=1 */
void fuzz_scalar_cond_negate_contract(const uint8_t *data, size_t size) {
    if (size >= 32) {
        int ret0, ret1;
        secp256k1_scalar a, r, neg;
        secp256k1_scalar_set_b32(&a, data, NULL);

        r = a;
        ret0 = secp256k1_scalar_cond_negate(&r, 0);
        CHECK(ret0 == 1);
        CHECK(secp256k1_scalar_eq(&r, &a));

        secp256k1_scalar_negate(&neg, &a);
        r = a;
        ret1 = secp256k1_scalar_cond_negate(&r, 1);
        CHECK(ret1 == -1);
        CHECK(secp256k1_scalar_eq(&r, &neg));
    }
}

/* If is_high(a), cond_negate(a,1) yields !is_high(result) */
void fuzz_scalar_high_normalization(const uint8_t *data, size_t size) {
    if (size >= 32) {
        secp256k1_scalar a, r;
        secp256k1_scalar_set_b32(&a, data, NULL);
        r = a;
        if (secp256k1_scalar_is_high(&a)) {
            (void)secp256k1_scalar_cond_negate(&r, 1);
            CHECK(!secp256k1_scalar_is_high(&r));
        } else {
            (void)secp256k1_scalar_cond_negate(&r, 0);
            CHECK(secp256k1_scalar_eq(&r, &a));
        }
    }
}

/* set_b32(get_b32(a)) == a */
void fuzz_scalar_b32_roundtrip(const uint8_t *data, size_t size) {
    if (size >= 32) {
        secp256k1_scalar a, b;
        unsigned char buf[32];
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_get_b32(buf, &a);
        secp256k1_scalar_set_b32(&b, buf, NULL);
        CHECK(secp256k1_scalar_eq(&a, &b));
    }
}

/* from_signed62(to_signed62(a)) == a */
void fuzz_scalar_signed62_roundtrip(const uint8_t *data, size_t size) {
    if (size >= 32) {
        secp256k1_scalar a, b;
        secp256k1_modinv64_signed62 s;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_to_signed62(&s, &a);
        secp256k1_scalar_from_signed62(&b, &s);
        CHECK(secp256k1_scalar_eq(&a, &b));
    }
}

/* mul(a,a) is stable and equals itself recomputed */
void fuzz_scalar_square_via_mul(const uint8_t *data, size_t size) {
    if (size >= 32) {
        secp256k1_scalar a, r1, r2;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_mul(&r1, &a, &a);
        secp256k1_scalar_mul(&r2, &a, &a);
        CHECK(secp256k1_scalar_eq(&r1, &r2));
    }
}

/* a*(b+c) == a*b + a*c */
void fuzz_scalar_mul_distributivity_right(const uint8_t *data, size_t size) {
    if (size >= 96) {
        secp256k1_scalar a, b, c, r1, r2, r3;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_set_b32(&b, data + 32, NULL);
        secp256k1_scalar_set_b32(&c, data + 64, NULL);
        secp256k1_scalar_add(&r1, &b, &c);
        secp256k1_scalar_mul(&r1, &a, &r1);
        secp256k1_scalar_mul(&r2, &a, &b);
        secp256k1_scalar_mul(&r3, &a, &c);
        secp256k1_scalar_add(&r2, &r2, &r3);
        CHECK(secp256k1_scalar_eq(&r1, &r2));
    }
}

/* get_bits_var matches limb mask for offsets < 32 and count <= 32-offset */
void fuzz_scalar_get_bits_consistency(const uint8_t *data, size_t size) {
    if (size >= 33) {
        unsigned off, maxc, cnt;
        uint32_t r1, r2;
        uint64_t limb0;
        secp256k1_scalar a;
        secp256k1_scalar_set_b32(&a, data, NULL);
        off = data[32] & 31;
        maxc = 32 - off;
        cnt = 1 + (data[31] % maxc);
        r1 = secp256k1_scalar_get_bits_var(&a, off, cnt);
        limb0 = a.d[0] >> off;
        r2 = (uint32_t)(limb0 & ((cnt == 32) ? 0xFFFFFFFFu : ((1u << cnt) - 1u)));
        CHECK(r1 == r2);
    }
}
