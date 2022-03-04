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



//for bn lshift
# define BN_FLG_FIXED_TOP 0
//void bn_correct_top(BIGNUM *a)
void bn_correct_top(BN_ULONG* a, int* top_a, int* flags_a, int* neg_a);


//for bn rshift
void BN_zero_ex(/*BN_ULONG* a,*/int* top_a, int* flags_a, int* neg_a);
#  define BN_zero(top_a, flags_a, neg_a)      BN_zero_ex(top_a, flags_a, neg_a)


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