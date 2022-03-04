#ifndef BITS_OPS_H_
#define BITS_OPS_H_
#include "botan.h"

word expand_top_bit(word a);
word ct_is_zero(word x);
word choose(word mask, word a, word b);


#endif