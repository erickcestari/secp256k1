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
        secp256k1_scalar a, r1;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_add(&r1, &a, &secp256k1_scalar_zero);
        CHECK(secp256k1_scalar_eq(&r1, &a));
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
        secp256k1_scalar a, r1;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_mul(&r1, &a, &secp256k1_scalar_zero);
        CHECK(secp256k1_scalar_is_zero(&r1));
    }
}

/* Test scalar inverse */
void fuzz_scalar_inverse(const uint8_t *data, size_t size) {
    if (size >= 32) {     
        secp256k1_scalar a, r1, r2, r3;
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
// void fuzz_scalar_shift(const uint8_t *data, size_t size) {
//     if (size >= 32) {
//         int bit, r1, r2;     
//         secp256k1_scalar a;
//         secp256k1_scalar_set_b32(&a, data, NULL);
//         bit = 1 + (data[31] % 15);
//         r2 = a.d[0] % (1ULL << bit);
//         r1 = secp256k1_scalar_shr_int(&a, bit);
//         CHECK(r1 == r2);
//     }
// }

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
        secp256k1_scalar a, b, r1;
        secp256k1_scalar_set_b32(&a, data, NULL);
        secp256k1_scalar_set_b32(&b, data + 32, NULL);
        int flag = size % 2;
        r1 = a;
        if (flag) {            
            secp256k1_scalar_cmov(&r1, &b, 1);
            CHECK(secp256k1_scalar_eq(&r1, &b));
        } else {
            secp256k1_scalar_cmov(&r1, &b, 0);
            CHECK(secp256k1_scalar_eq(&r1, &a));
        }
    }
}
