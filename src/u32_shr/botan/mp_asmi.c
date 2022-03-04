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

//#define BOTAN_MP_USE_X86_64_ASM

#define ADD_OR_SUBTRACT(CORE_CODE)     \
        ASM("rorq %[carry]")           \
        CORE_CODE                      \
        ASM("sbbq %[carry],%[carry]")  \
        ASM("negq %[carry]")

#define ASM(x) x "\n\t"

/*
* Word Subtraction
*/
//word word_sub(word x, word y, word* carry)
void word_sub(word x, word y, word* carry, word* z)
   {
#if defined(BOTAN_MP_USE_X86_32_ASM)
   asm(
      ADD_OR_SUBTRACT(ASM("sbbl %[y],%[x]"))
      : [x]"=r"(x), [carry]"=r"(*carry)
      : "0"(x), [y]"rm"(y), "1"(*carry)
      : "cc");
   return x;

#elif defined(BOTAN_MP_USE_X86_64_ASM)
   asm(
      ADD_OR_SUBTRACT(ASM("sbbq %[y],%[x]"))
      : [x]"=r"(x), [carry]"=r"(*carry)
      : "0"(x), [y]"rm"(y), "1"(*carry)
      : "cc");
   return x;

#else
   word t0 = x - y;
   word c1 = (t0 > x);
   word ztmp = t0 - *carry;
   *carry = c1 | (ztmp > t0);
   *z = ztmp;
   //return z;
#endif
   }

/*
* Eight Word Block Subtraction, Three Argument
*/
word word8_sub3(word z[8], const word x[8],
                       const word y[8], word carry)
   {
#if defined(BOTAN_MP_USE_X86_32_ASM)
   asm(
      ADD_OR_SUBTRACT(DO_8_TIMES(ADDSUB3_OP, "sbbl"))
      : [carry]"=r"(carry)
      : [x]"r"(x), [y]"r"(y), [z]"r"(z), "0"(carry)
      : "cc", "memory");

#elif defined(BOTAN_MP_USE_X86_64_ASM)
   asm(
      ADD_OR_SUBTRACT(DO_8_TIMES(ADDSUB3_OP, "sbbq"))
      : [carry]"=r"(carry)
      : [x]"r"(x), [y]"r"(y), [z]"r"(z), "0"(carry)
      : "cc", "memory");

#else
    /*
   z[0] = word_sub(x[0], y[0], &carry);
   z[1] = word_sub(x[1], y[1], &carry);
   z[2] = word_sub(x[2], y[2], &carry);
   z[3] = word_sub(x[3], y[3], &carry);
   z[4] = word_sub(x[4], y[4], &carry);
   z[5] = word_sub(x[5], y[5], &carry);
   z[6] = word_sub(x[6], y[6], &carry);
   z[7] = word_sub(x[7], y[7], &carry);
   */
    word_sub(x[0], y[0], &carry, z + 0);
    word_sub(x[0], y[0], &carry, z + 1);
    word_sub(x[0], y[0], &carry, z + 2);
    word_sub(x[0], y[0], &carry, z + 3);
    word_sub(x[0], y[0], &carry, z + 4);
    word_sub(x[0], y[0], &carry, z + 5);
    word_sub(x[0], y[0], &carry, z + 6);
    word_sub(x[0], y[0], &carry, z + 7);
#endif

   return carry;
   }

/**
* Perform a 64x64->128 bit multiplication
*/
void mul64x64_128(uint64_t a, uint64_t b, uint64_t* lo, uint64_t* hi)
{
#if defined(BOTAN_TARGET_HAS_NATIVE_UINT128)

    const uint128_t r = static_cast<uint128_t>(a) * b;
    *hi = (r >> 64) & 0xFFFFFFFFFFFFFFFF;
    *lo = (r) & 0xFFFFFFFFFFFFFFFF;

#elif defined(BOTAN_BUILD_COMPILER_IS_MSVC) && defined(BOTAN_TARGET_CPU_HAS_NATIVE_64BIT)
    *lo = _umul128(a, b, hi);

#elif defined(BOTAN_USE_GCC_INLINE_ASM) && defined(BOTAN_TARGET_ARCH_IS_X86_64)
    asm("mulq %3"
        : "=d" (*hi), "=a" (*lo)
        : "a" (a), "rm" (b)
        : "cc");

#elif defined(BOTAN_USE_GCC_INLINE_ASM) && defined(BOTAN_TARGET_ARCH_IS_PPC64)
    asm("mulhdu %0,%1,%2"
        : "=r" (*hi)
        : "r" (a), "r" (b)
        : "cc");
    *lo = a * b;

#else

    /*
    * Do a 64x64->128 multiply using four 32x32->64 multiplies plus
    * some adds and shifts. Last resort for CPUs like UltraSPARC (with
    * 64-bit registers/ALU, but no 64x64->128 multiply) or 32-bit CPUs.
    */
    const size_t HWORD_BITS = 32;
    const uint32_t HWORD_MASK = 0xFFFFFFFF;

    const uint32_t a_hi = (a >> HWORD_BITS);
    const uint32_t a_lo = (a & HWORD_MASK);
    const uint32_t b_hi = (b >> HWORD_BITS);
    const uint32_t b_lo = (b & HWORD_MASK);

    uint64_t x0 = (uint64_t)(a_hi) * b_hi;
    uint64_t x1 = (uint64_t)(a_lo) * b_hi;
    uint64_t x2 = (uint64_t)(a_hi) * b_lo;
    uint64_t x3 = (uint64_t)(a_lo) * b_lo;

    // this cannot overflow as (2^32-1)^2 + 2^32-1 < 2^64-1
    x2 += x3 >> HWORD_BITS;

    // this one can overflow
    x2 += x1;

    // propagate the carry if any
    x0 += (uint64_t)(x2 < x1) << HWORD_BITS;

    *hi = x0 + (x2 >> HWORD_BITS);
    *lo = ((x2 & HWORD_MASK) << HWORD_BITS) + (x3 & HWORD_MASK);
#endif
}

/*
* Word Multiply/Add
*/
word word_madd2(word a, word b, word* c)
{
#if defined(BOTAN_MP_USE_X86_32_ASM)
    asm(R"(
      mull %[b]
      addl %[c],%[a]
      adcl $0,%[carry]
      )"
        : [a] "=a"(a), [b]"=rm"(b), [carry]"=&d"(*c)
        : "0"(a), "1"(b), [c]"g"(*c) : "cc");

    return a;

#elif defined(BOTAN_MP_USE_X86_64_ASM)
    asm(R"(
      mulq %[b]
      addq %[c],%[a]
      adcq $0,%[carry]
      )"
        : [a] "=a"(a), [b]"=rm"(b), [carry]"=&d"(*c)
        : "0"(a), "1"(b), [c]"g"(*c) : "cc");

    return a;

#elif defined(BOTAN_HAS_MP_DWORD)
    const dword s = static_cast<dword>(a) * b + *c;
    *c = static_cast<word>(s >> BOTAN_MP_WORD_BITS);
    return static_cast<word>(s);
#else
    // static_assert(BOTAN_MP_WORD_BITS == 64, "Unexpected word size");

    word hi = 0, lo = 0;

    mul64x64_128(a, b, &lo, &hi);

    lo += *c;
    hi += (lo < *c); // carry?

    *c = hi;
    return lo;
#endif
}

/*
* Multiply-Add Accumulator
* (w2,w1,w0) += x * y
*/
void word3_muladd(word* w2, word* w1, word* w0, word x, word y)
{
#if defined(BOTAN_MP_USE_X86_32_ASM)
    word z0 = 0, z1 = 0;

    asm("mull %[y]"
        : "=a"(z0), "=d"(z1)
        : "a"(x), [y]"rm"(y)
        : "cc");

    asm(R"(
       addl %[z0],%[w0]
       adcl %[z1],%[w1]
       adcl $0,%[w2]
       )"
        : [w0] "=r"(*w0), [w1]"=r"(*w1), [w2]"=r"(*w2)
        : [z0] "r"(z0), [z1]"r"(z1), "0"(*w0), "1"(*w1), "2"(*w2)
        : "cc");

#elif defined(BOTAN_MP_USE_X86_64_ASM)
    word z0 = 0, z1 = 0;

    asm("mulq %[y]"
        : "=a"(z0), "=d"(z1)
        : "a"(x), [y]"rm"(y)
        : "cc");

    asm(R"(
       addq %[z0],%[w0]
       adcq %[z1],%[w1]
       adcq $0,%[w2]
       )"
        : [w0] "=r"(*w0), [w1]"=r"(*w1), [w2]"=r"(*w2)
        : [z0] "r"(z0), [z1]"r"(z1), "0"(*w0), "1"(*w1), "2"(*w2)
        : "cc");

#else
    word carry = *w0;
    *w0 = word_madd2(x, y, &carry);
    *w1 += carry;
    *w2 += (*w1 < carry);
#endif
}


/*
* Multiply-Add Accumulator
* (w2,w1,w0) += 2 * x * y
*/
void word3_muladd_2(word* w2, word* w1, word* w0, word x, word y)
{
#if defined(BOTAN_MP_USE_X86_32_ASM)
    word z0 = 0, z1 = 0;

    asm("mull %[y]"
        : "=a"(z0), "=d"(z1)
        : "a"(x), [y]"rm"(y)
        : "cc");

    asm(R"(
      addl %[z0],%[w0]
      adcl %[z1],%[w1]
      adcl $0,%[w2]
      addl %[z0],%[w0]
      adcl %[z1],%[w1]
      adcl $0,%[w2]
      )"
        : [w0] "=r"(*w0), [w1]"=r"(*w1), [w2]"=r"(*w2)
        : [z0] "r"(z0), [z1]"r"(z1), "0"(*w0), "1"(*w1), "2"(*w2)
        : "cc");

#elif defined(BOTAN_MP_USE_X86_64_ASM)
    word z0 = 0, z1 = 0;

    asm("mulq %[y]"
        : "=a"(z0), "=d"(z1)
        : "a"(x), [y]"rm"(y)
        : "cc");

    asm(R"(
      addq %[z0],%[w0]
      adcq %[z1],%[w1]
      adcq $0,%[w2]
      addq %[z0],%[w0]
      adcq %[z1],%[w1]
      adcq $0,%[w2]
      )"
        : [w0] "=r"(*w0), [w1]"=r"(*w1), [w2]"=r"(*w2)
        : [z0] "r"(z0), [z1]"r"(z1), "0"(*w0), "1"(*w1), "2"(*w2)
        : "cc");

#else
    word carry = 0;
    x = word_madd2(x, y, &carry);
    y = carry;

    word top = (y >> (BOTAN_MP_WORD_BITS - 1));
    y <<= 1;
    y |= (x >> (BOTAN_MP_WORD_BITS - 1));
    x <<= 1;

    carry = 0;
    *w0 = word_add(*w0, x, &carry);
    *w1 = word_add(*w1, y, &carry);
    *w2 = word_add(*w2, top, &carry);
#endif
}


