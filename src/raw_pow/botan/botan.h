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
void raw_pow(word a[4], word r[8]);

#endif