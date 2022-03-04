In file included from BN_mul.c:1:
bn.h:94:3: warning: multi-line comment [-Wcomment]
   94 |   //# define ossl_assert(x) ((x) != 0){\
      |   ^
bn.h:99:3: warning: multi-line comment [-Wcomment]
   99 |   //#  define bn_check_top(a) \
      |   ^
bn.h:111:3: warning: multi-line comment [-Wcomment]
  111 |   //#  define bn_check_top(top_a,flags_a,neg_a) \
      |   ^
BN_mul.c: In function ‘word3_muladd’:
BN_mul.c:4:5: warning: implicit declaration of function ‘mul_add_c’; did you mean ‘mul_add_c_ref’? [-Wimplicit-function-declaration]
    4 |     mul_add_c(a, b, c0, c1, c2);
      |     ^~~~~~~~~
      |     mul_add_c_ref
BN_mul.c: At top level:
BN_mul.c:7:6: warning: conflicting types for ‘mul_add_c’
    7 | void mul_add_c(BN_ULONG a, BN_ULONG b, BN_ULONG* c0, BN_ULONG* c1, BN_ULONG* c2) {
      |      ^~~~~~~~~
BN_mul.c:4:5: note: previous implicit declaration of ‘mul_add_c’ was here
    4 |     mul_add_c(a, b, c0, c1, c2);
      |     ^~~~~~~~~
BN_mul.c: In function ‘bn_mul_fixed_top’:
BN_mul.c:88:9: warning: variable ‘top’ set but not used [-Wunused-but-set-variable]
   88 |     int top;// , al, bl;
      |         ^~~
BN_mul.c:78:9: warning: unused variable ‘dmax_b’ [-Wunused-variable]
   78 |     int dmax_b = 4;
      |         ^~~~~~
BN_mul.c:77:9: warning: unused variable ‘dmax_a’ [-Wunused-variable]
   77 |     int dmax_a = 4;
      |         ^~~~~~
BN_mul.c:71:9: warning: unused variable ‘flags_b’ [-Wunused-variable]
   71 |     int flags_b = BN_FLG_SECURE;
      |         ^~~~~~~
BN_mul.c:70:9: warning: unused variable ‘flags_a’ [-Wunused-variable]
   70 |     int flags_a = BN_FLG_SECURE;
      |         ^~~~~~~
In file included from BN_mul.c:1:
BN_mul.c: At top level:
bn.h:313:13: warning: ‘bn_free_d’ declared ‘static’ but never defined [-Wunused-function]
  313 | static void bn_free_d(BN_ULONG* a, int flags_a, int dmax_a,
      |             ^~~~~~~~~
bn.h:316:18: warning: ‘bn_expand_internal’ declared ‘static’ but never defined [-Wunused-function]
  316 | static BN_ULONG* bn_expand_internal(const BN_ULONG* b, int top_b, int flags_b, /*int neg_b,*/
      |                  ^~~~~~~~~~~~~~~~~~
