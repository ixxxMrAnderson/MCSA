#include "mp_asmi.h"

/*
* Word Addition
*/
word word_add(word x, word y, word* carry)
   {
#if defined(BOTAN_MP_USE_X86_32_ASM)
   asm(
      ADD_OR_SUBTRACT(ASM("adcl %[y],%[x]"))
      : [x]"=r"(x), [carry]"=r"(*carry)
      : "0"(x), [y]"rm"(y), "1"(*carry)
      : "cc");
   return x;

#elif defined(BOTAN_MP_USE_X86_64_ASM)

   asm(
      ADD_OR_SUBTRACT(ASM("adcq %[y],%[x]"))
      : [x]"=r"(x), [carry]"=r"(*carry)
      : "0"(x), [y]"rm"(y), "1"(*carry)
      : "cc");
   return x;

#else
   word z = x + y;
   word c1 = (z < x);
   z += *carry;
   *carry = c1 | (z < *carry);
   return z;
#endif
   }

word word8_add2(word x[8], const word y[8], word carry)
   {
#if defined(BOTAN_MP_USE_X86_32_ASM)
   asm(
      ADD_OR_SUBTRACT(DO_8_TIMES(ADDSUB2_OP, "adcl"))
      : [carry]"=r"(carry)
      : [x]"r"(x), [y]"r"(y), "0"(carry)
      : "cc", "memory");

#elif defined(BOTAN_MP_USE_X86_64_ASM)

   asm(
      ADD_OR_SUBTRACT(DO_8_TIMES(ADDSUB2_OP, "adcq"))
      : [carry]"=r"(carry)
      : [x]"r"(x), [y]"r"(y), "0"(carry)
      : "cc", "memory");

#else
   x[0] = word_add(x[0], y[0], &carry);
   x[1] = word_add(x[1], y[1], &carry);
   x[2] = word_add(x[2], y[2], &carry);
   x[3] = word_add(x[3], y[3], &carry);
   x[4] = word_add(x[4], y[4], &carry);
   x[5] = word_add(x[5], y[5], &carry);
   x[6] = word_add(x[6], y[6], &carry);
   x[7] = word_add(x[7], y[7], &carry);
#endif

   return carry;
   }

/*
* Eight Word Block Addition, Three Argument
*/
word word8_add3(word z[8], const word x[8],
    const word y[8], word carry)
{
#if defined(BOTAN_MP_USE_X86_32_ASM)
    asm(
        ADD_OR_SUBTRACT(DO_8_TIMES(ADDSUB3_OP, "adcl"))
        : [carry] "=r"(carry)
        : [x] "r"(x), [y]"r"(y), [z]"r"(z), "0"(carry)
        : "cc", "memory");

#elif defined(BOTAN_MP_USE_X86_64_ASM)
    asm(
        ADD_OR_SUBTRACT(DO_8_TIMES(ADDSUB3_OP, "adcq"))
        : [carry] "=r"(carry)
        : [x] "r"(x), [y]"r"(y), [z]"r"(z), "0"(carry)
        : "cc", "memory");

#else
    z[0] = word_add(x[0], y[0], &carry);
    z[1] = word_add(x[1], y[1], &carry);
    z[2] = word_add(x[2], y[2], &carry);
    z[3] = word_add(x[3], y[3], &carry);
    z[4] = word_add(x[4], y[4], &carry);
    z[5] = word_add(x[5], y[5], &carry);
    z[6] = word_add(x[6], y[6], &carry);
    z[7] = word_add(x[7], y[7], &carry);
#endif

    return carry;
}
