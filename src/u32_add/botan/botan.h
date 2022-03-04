#ifndef BOTAN_H_
#define BOTAN_H_

#include <stdint.h>
#include <stddef.h>

typedef uint64_t word;


//void bigint_add2(word x[], size_t x_size, const word y[], size_t y_size);
void u32_add(word z[4], const word x[4], const word y[4]);

#endif