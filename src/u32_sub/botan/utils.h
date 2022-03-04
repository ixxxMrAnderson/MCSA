#ifndef UTILS_H_
#define UTILS_H_

#include "botan.h"

word expand(word a);
void copy_mem(word* out, const word* in, size_t n);
void clear_mem(word* ptr, size_t n);

size_t min(size_t a, size_t b);

int sized_for_comba_mul(size_t x_sw, size_t x_size,
    size_t y_sw, size_t y_size,
    size_t z_size);

int sized_for_comba_sqr(size_t x_sw, size_t x_size,
    size_t z_size);


#endif