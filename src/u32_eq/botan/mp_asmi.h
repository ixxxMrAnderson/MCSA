#ifndef MP_ASMI_H_
#define MP_ASMI_H_

#include "botan.h"

word word_add(word x, word y, word* carry);
word word8_add2(word x[8], const word y[8], word carry);
word word8_add3(word z[8], const word x[8], const word y[8], word carry);

//word word_sub(word x, word y, word* carry);
void word_sub(word x, word y, word* carry, word* z);
word word8_sub3(word z[8], const word x[8], const word y[8], word carry);

void word3_muladd(word* w2, word* w1, word* w0, word x, word y);
void word3_muladd_2(word* w2, word* w1, word* w0, word x, word y);

#endif