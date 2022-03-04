#ifndef MP_ASMI_H_
#define MP_ASMI_H_

#include "botan.h"

word word_add(word x, word y, word* carry);
word word8_add2(word x[8], const word y[8], word carry);
word word8_add3(word z[8], const word x[8], const word y[8], word carry);

#endif