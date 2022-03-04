#ifndef BOTAN_H_
#define BOTAN_H_

#include <stdint.h>
#include <stddef.h>
#define BOTAN_MP_WORD_BITS 64
typedef uint64_t word;


//void bigint_add2(word x[], size_t x_size, const word y[], size_t y_size);
void u32_add(word r[4], const word a[4], const word b[4]);
void u32_sub(word r[4], const word a[4], const word b[4]);

void u32_shl(word a[4]);
void u32_shr(word a[4]);

void u32_mul(word a[4], word b[4], word r[8]);
void u32_sqr(word a[4], word r[8]);

int32_t cmp(const word* x, const word* y);
uint8_t u32_eq(const word* a, const word* b);
uint8_t u32_gte(const word* a, const word* b);
uint8_t u32_eq_one(const word* a);
uint8_t u32_eq_zero(const word* a);


#endif