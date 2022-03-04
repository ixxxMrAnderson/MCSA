#pragma once


#ifndef _BN_H_
#define _BN_H_

#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>



#define SIXTY_FOUR_BIT

# ifdef SIXTY_FOUR_BIT_LONG
#  define BN_ULONG        unsigned long
#  define BN_BYTES        8
# endif

# ifdef SIXTY_FOUR_BIT
#  define BN_ULONG        unsigned long long
#  define BN_BYTES        8
# endif

# ifdef THIRTY_TWO_BIT
#  define BN_ULONG        unsigned int
#  define BN_BYTES        4
# endif

# define BN_BITS2       (BN_BYTES * 8)
# define BN_BITS        (BN_BITS2 * 2)
# define BN_TBIT        ((BN_ULONG)1 << (BN_BITS2 - 1))



/*
 * 64-bit processor with LP64 ABI
 */
# ifdef SIXTY_FOUR_BIT_LONG
#  define BN_ULLONG       unsigned long long
#  define BN_BITS4        32
#  define BN_MASK2        (0xffffffffffffffffL)
#  define BN_MASK2l       (0xffffffffL)
#  define BN_MASK2h       (0xffffffff00000000L)
#  define BN_MASK2h1      (0xffffffff80000000L)
#  define BN_DEC_CONV     (10000000000000000000UL)
#  define BN_DEC_NUM      19
#  define BN_DEC_FMT1     "%lu"
#  define BN_DEC_FMT2     "%019lu"
# endif

 /*
  * 64-bit processor other than LP64 ABI
  */
# ifdef SIXTY_FOUR_BIT
#  undef BN_LLONG
#  undef BN_ULLONG
#  define BN_BITS4        32
#  define BN_MASK2        (0xffffffffffffffffLL)
#  define BN_MASK2l       (0xffffffffL)
#  define BN_MASK2h       (0xffffffff00000000LL)
#  define BN_MASK2h1      (0xffffffff80000000LL)
#  define BN_DEC_CONV     (10000000000000000000ULL)
#  define BN_DEC_NUM      19
#  define BN_DEC_FMT1     "%llu"
#  define BN_DEC_FMT2     "%019llu"
# endif

# ifdef THIRTY_TWO_BIT
#  ifdef BN_LLONG
#   if defined(_WIN32) && !defined(__GNUC__)
#    define BN_ULLONG     unsigned __int64
#   else
#    define BN_ULLONG     unsigned long long
#   endif
#  endif
#  define BN_BITS4        16
#  define BN_MASK2        (0xffffffffL)
#  define BN_MASK2l       (0xffff)
#  define BN_MASK2h1      (0xffff8000L)
#  define BN_MASK2h       (0xffff0000L)
#  define BN_DEC_CONV     (1000000000L)
#  define BN_DEC_NUM      9
#  define BN_DEC_FMT1     "%u"
#  define BN_DEC_FMT2     "%09u"
# endif



  //# define ossl_assert(x) ((x) != 0){\
          printf("no\n");\
          exit(0);\
  }

  //#  define bn_check_top(a) \
          do { \
                  const BIGNUM *_bnum2 = (a); \
                  if (_bnum2 != NULL) { \
                          int _top = _bnum2->top; \
                          (void)ossl_assert((_top == 0 && !_bnum2->neg) || \
                                    (_top && ((_bnum2->flags & BN_FLG_FIXED_TOP) \
                                              || _bnum2->d[_top - 1] != 0))); \
                          bn_pollute(_bnum2); \
                  } \
          } while(0)

  //#  define bn_check_top(top_a,flags_a,neg_a) \
          do { \
                      int _top = top_a; \
                      (void)ossl_assert((_top == 0 && !neg_a) || \
                                (_top && ((flags_a & BN_FLG_FIXED_TOP) \
                                          || _bnum2->d[_top - 1] != 0))); \
                  } \
          } while(0)
#  define bn_check_top(a)
#  define bn_pollute(a)

//struct bignum_st
//       {
//       BN_ULONG *d;    /* Pointer to an array of 'BN_BITS2' bit chunks. */
//       int top;        /* Index of last used d +1. */
//       /* The next are internal book keeping for bn_expand. */
//      int dmax;       /* Size of the d array. */
      // int neg;        /* one if the number is negative */
      // int flags;
       // };

# define BN_FLG_STATIC_DATA      0x02
# define BN_FLG_SECURE           0x08
#define BN_get_flags(_flags, _mask) ((_flags) & _mask)
#define OPENSSL_secure_zalloc(a) (BN_ULONG*)malloc((a))
#define OPENSSL_zalloc(a) (BN_ULONG*)malloc((a))



/*---------------------------------------------BN_UMULT ºê¶¨Òå-----------------------------------------*/
//#define __x86_64
//#define __GNUC__ 2
//#define asm __asm
#define __SIZEOF_INT128__ 16
#define uint128_t __int128_t


# if !defined(OPENSSL_NO_ASM) && !defined(OPENSSL_NO_INLINE_ASM) && !defined(PEDANTIC)
/*
 * BN_UMULT_HIGH section.
 * If the compiler doesn't support 2*N integer type, then you have to
 * replace every N*N multiplication with 4 (N/2)*(N/2) accompanied by some
 * shifts and additions which unavoidably results in severe performance
 * penalties. Of course provided that the hardware is capable of producing
 * 2*N result... That's when you normally start considering assembler
 * implementation. However! It should be pointed out that some CPUs (e.g.,
 * PowerPC, Alpha, and IA-64) provide *separate* instruction calculating
 * the upper half of the product placing the result into a general
 * purpose register. Now *if* the compiler supports inline assembler,
 * then it's not impossible to implement the "bignum" routines (and have
 * the compiler optimize 'em) exhibiting "native" performance in C. That's
 * what BN_UMULT_HIGH macro is about:-) Note that more recent compilers do
 * support 2*64 integer type, which is also used here.
 */
#  if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__==16 && \
      (defined(SIXTY_FOUR_BIT) || defined(SIXTY_FOUR_BIT_LONG))
#   define BN_UMULT_HIGH(a,b)          (((uint128_t)(a)*(b))>>64)
#   define BN_UMULT_LOHI(low,high,a,b) ({       \
        uint128_t ret=(uint128_t)(a)*(b);   \
        (high)=ret>>64; (low)=ret;      })
#  elif defined(__alpha) && (defined(SIXTY_FOUR_BIT_LONG) || defined(SIXTY_FOUR_BIT))
#   if defined(__DECC)
#    include <c_asm.h>
#    define BN_UMULT_HIGH(a,b)   (BN_ULONG)asm("umulh %a0,%a1,%v0",(a),(b))
#   elif defined(__GNUC__) && __GNUC__>=2
#    define BN_UMULT_HIGH(a,b)   ({     \
        register BN_ULONG ret;          \
        asm ("umulh     %1,%2,%0"       \
             : "=r"(ret)                \
             : "r"(a), "r"(b));         \
        ret;                      })
#   endif                       /* compiler */
#  elif defined(_ARCH_PPC64) && defined(SIXTY_FOUR_BIT_LONG)
#   if defined(__GNUC__) && __GNUC__>=2
#    define BN_UMULT_HIGH(a,b)   ({     \
        register BN_ULONG ret;          \
        asm ("mulhdu    %0,%1,%2"       \
             : "=r"(ret)                \
             : "r"(a), "r"(b));         \
        ret;                      })
#   endif                       /* compiler */
#  elif (defined(__x86_64) || defined(__x86_64__)) && \
       (defined(SIXTY_FOUR_BIT_LONG) || defined(SIXTY_FOUR_BIT))
#   if defined(__GNUC__) && __GNUC__>=2
#    define BN_UMULT_HIGH(a,b)   ({     \
        register BN_ULONG ret,discard;  \
        asm ("mulq      %3"             \
             : "=a"(discard),"=d"(ret)  \
             : "a"(a), "g"(b)           \
             : "cc");                   \
        ret;                      })
#    define BN_UMULT_LOHI(low,high,a,b) \
        asm ("mulq      %3"             \
                : "=a"(low),"=d"(high)  \
                : "a"(a),"g"(b)         \
                : "cc");
#   endif
#  elif (defined(_M_AMD64) || defined(_M_X64)) && defined(SIXTY_FOUR_BIT)
#   if defined(_MSC_VER) && _MSC_VER>=1400
unsigned __int64 __umulh(unsigned __int64 a, unsigned __int64 b);
unsigned __int64 _umul128(unsigned __int64 a, unsigned __int64 b,
    unsigned __int64* h);
#    pragma intrinsic(__umulh,_umul128)
#    define BN_UMULT_HIGH(a,b)           __umulh((a),(b))
#    define BN_UMULT_LOHI(low,high,a,b)  ((low)=_umul128((a),(b),&(high)))
#   endif
#  elif defined(__mips) && (defined(SIXTY_FOUR_BIT) || defined(SIXTY_FOUR_BIT_LONG))
#   if defined(__GNUC__) && __GNUC__>=2
#    define BN_UMULT_HIGH(a,b) ({       \
        register BN_ULONG ret;          \
        asm ("dmultu    %1,%2"          \
             : "=h"(ret)                \
             : "r"(a), "r"(b) : "l");   \
        ret;                    })
#    define BN_UMULT_LOHI(low,high,a,b) \
        asm ("dmultu    %2,%3"          \
             : "=l"(low),"=h"(high)     \
             : "r"(a), "r"(b));
#   endif
#  elif defined(__aarch64__) && defined(SIXTY_FOUR_BIT_LONG)
#   if defined(__GNUC__) && __GNUC__>=2
#    define BN_UMULT_HIGH(a,b)   ({     \
        register BN_ULONG ret;          \
        asm ("umulh     %0,%1,%2"       \
             : "=r"(ret)                \
             : "r"(a), "r"(b));         \
        ret;                      })
#   endif
#  endif                        /* cpu */
# endif                         /* OPENSSL_NO_ASM */

/*----------------------------------------------------------------------------------------------------*/




int BN_uadd(BN_ULONG* r, int* top_r, int* flags_r, int* neg_r, int* dmax_r,
    const BN_ULONG* a, int top_a, int flags_a, int neg_a, int dmax_a,
    const BN_ULONG* b, int top_b, int flags_b, int neg_b, int dmax_b);
int u32_add(BN_ULONG* r, /*int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    const BN_ULONG* a, /*int top_a, int flags_a, int neg_a, int dmax_a,*/
    const BN_ULONG* b);
BN_ULONG* u32_add_ret(/*BN_ULONG* r,*/ /*int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    const BN_ULONG* a, /*int top_a, int flags_a, int neg_a, int dmax_a,*/
    const BN_ULONG* b);


int BN_usub(BN_ULONG* r, int* top_r, int* flags_r, int* neg_r, int* dmax_r,
    const BN_ULONG* a, int top_a, int flags_a, int neg_a, int dmax_a,
    const BN_ULONG* b, int top_b, int flags_b, int neg_b, int dmax_b);
int u32_sub(BN_ULONG* r,/* int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    const BN_ULONG* a, /*int top_a, int flags_a, int neg_a, int dmax_a,*/
    const BN_ULONG* b/*, int top_b, int flags_b, int neg_b, int dmax_b*/);
BN_ULONG* u32_sub_ret(/*BN_ULONG* r,*//* int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    const BN_ULONG* a, /*int top_a, int flags_a, int neg_a, int dmax_a,*/
    const BN_ULONG* b/*, int top_b, int flags_b, int neg_b, int dmax_b*/);


int BN_lshift(BN_ULONG* r, int* top_r, int* flags_r, int* neg_r, int* dmax_r,
    const BN_ULONG* a, int top_a, int flags_a, int neg_a, int dmax_a,
    int n);
int u32_shl(BN_ULONG* r,/* int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    const BN_ULONG* a, /*int top_a, int flags_a, int neg_a, int dmax_a,*/
    int n);
int u32_shl_self(BN_ULONG* r,/* int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    /*const BN_ULONG* a,*/ /*int top_a, int flags_a, int neg_a, int dmax_a,*/
    int n);

int BN_rshift(BN_ULONG* r, int* top_r, int* flags_r, int* neg_r, int* dmax_r,
    const BN_ULONG* a, int top_a, int flags_a, int neg_a, int dmax_a,
    int n);
int u32_shr(BN_ULONG* r, /*int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    const BN_ULONG* a,/* int top_a, int flags_a, int neg_a, int dmax_a,*/
    int n);
int u32_shr_self(BN_ULONG* r, /*int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    /*const BN_ULONG* a,*//* int top_a, int flags_a, int neg_a, int dmax_a,*/
    int n);


//for bn lshift
# define BN_FLG_FIXED_TOP 0
//void bn_correct_top(BIGNUM *a)
void bn_correct_top(BN_ULONG* a, int* top_a, int* flags_a, int* neg_a);


//for bn rshift
void BN_zero_ex(/*BN_ULONG* a,*/int* top_a, int* flags_a, int* neg_a);
#  define BN_zero(top_a, flags_a, neg_a)      BN_zero_ex(top_a, flags_a, neg_a)


int raw_mul(BN_ULONG* a, BN_ULONG* b, BN_ULONG* r);

//void mul_add_c(BN_ULONG a, BN_ULONG b, BN_ULONG* c0, BN_ULONG* c1, BN_ULONG* c2);
void mul_add_c_ref(BN_ULONG* c0, BN_ULONG* c1, BN_ULONG* c2, BN_ULONG a, BN_ULONG b);

/*----------------------------------- general utilities ----------------------------*/
void OPENSSL_secure_clear_free(BN_ULONG* a, size_t size);
void OPENSSL_clear_free(BN_ULONG* a, size_t size);

void OPENSSL_free(BN_ULONG* a);


static void bn_free_d(BN_ULONG* a, int flags_a, int dmax_a,
    int clear);

static BN_ULONG* bn_expand_internal(const BN_ULONG* b, int top_b, int flags_b, /*int neg_b,*/
    int words);


BN_ULONG* bn_expand2(BN_ULONG* b, int top_b, int flags_b, /*int neg_b,*/ int* dmax_b,
    /*BN_ULONG *r, int top_r, int flags_r, int neg_r, int dmax_r,*/
    int words);


BN_ULONG* bn_wexpand(BN_ULONG* a, int top_a, int flags_a, int neg_a, int* dmax_a,
    /*BN_ULONG *r, int top_r, int flags_r, int neg_r, int dmax_r,*/
    int words);

int cal_top(BN_ULONG* a, int dmax_a);

















#endif