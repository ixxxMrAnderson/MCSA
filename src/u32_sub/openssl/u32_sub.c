#include "bn.h"

BN_ULONG bn_sub_words(BN_ULONG* r, const BN_ULONG* a, const BN_ULONG* b,
    int n)
{
    BN_ULONG t1, t2;
    int c = 0;

    assert(n >= 0);
    if (n <= 0)
        return (BN_ULONG)0;

#ifndef OPENSSL_SMALL_FOOTPRINT
    while (n & ~3) {
        t1 = a[0];
        t2 = b[0];
        r[0] = (t1 - t2 - c) & BN_MASK2;
        if (t1 != t2)
            c = (t1 < t2);
        t1 = a[1];
        t2 = b[1];
        r[1] = (t1 - t2 - c) & BN_MASK2;
        if (t1 != t2)
            c = (t1 < t2);
        t1 = a[2];
        t2 = b[2];
        r[2] = (t1 - t2 - c) & BN_MASK2;
        if (t1 != t2)
            c = (t1 < t2);
        t1 = a[3];
        t2 = b[3];
        r[3] = (t1 - t2 - c) & BN_MASK2;
        if (t1 != t2)
            c = (t1 < t2);
        a += 4;
        b += 4;
        r += 4;
        n -= 4;
    }
#endif
    while (n) {
        t1 = a[0];
        t2 = b[0];
        r[0] = (t1 - t2 - c) & BN_MASK2;
        if (t1 != t2)
            c = (t1 < t2);
        a++;
        b++;
        r++;
        n--;
    }
    return c;
}


/* unsigned subtraction of b from a, a must be larger than b. */
//int BN_usub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b)
// int BN_usub(BN_ULONG* r, /*int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
//     const BN_ULONG* a, /*int top_a, int flags_a, int neg_a, int dmax_a,*/
//     const BN_ULONG* b/*, int top_b, int flags_b, int neg_b, int dmax_b*/)
// {

//     int flags_r = 0; //BN_FLG_SECURE;
//     int flags_a = BN_FLG_SECURE;
//     int flags_b = BN_FLG_SECURE;

//     int neg_r = 0;
//     int neg_a = 0;
//     int neg_b = 0;

//     int dmax_a = 4;
//     int dmax_b = 4;
//     int dmax_r = 0;

//     int top_a = cal_top(a, dmax_a);
//     int top_b = cal_top(b, dmax_b);
//     int top_r = 0;

//     int max, min, dif;
//     BN_ULONG t1, t2, borrow, * rp;
//     const BN_ULONG* ap, * bp;

//     bn_check_top(a);
//     bn_check_top(b);

//     max = top_a;
//     min = top_b;
//     dif = max - min;

//     if (dif < 0) {              /* hmm... should not be happening */
//         //ERR_raise(ERR_LIB_BN, BN_R_ARG2_LT_ARG3);
//         printf("error\n");
//         return 0;
//     }

//     bn_wexpand(r, top_r, flags_r, neg_r, &dmax_r, max);
//     if (r == NULL/*bn_wexpand(r, max) == NULL*/)
//         return 0;

//     ap = a;
//     bp = b;
//     rp = r;

//     borrow = bn_sub_words(rp, ap, bp, min);
//     ap += min;
//     rp += min;

//     while (dif) {
//         dif--;
//         t1 = *(ap++);
//         t2 = (t1 - borrow) & BN_MASK2;
//         *(rp++) = t2;
//         borrow &= (t1 == 0);
//     }

//     while (max && *--rp == 0)
//         max--;

//     top_r = max;
//     neg_r = 0;
//     bn_pollute(r);

//     return 1;
// }



int u32_sub(BN_ULONG* r,/* int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    const BN_ULONG* a, /*int top_a, int flags_a, int neg_a, int dmax_a,*/
    const BN_ULONG* b/*, int top_b, int flags_b, int neg_b, int dmax_b*/)
{
    BN_ULONG* init_r = r;
    int flags_r = 0; //BN_FLG_SECURE;
    int flags_a = BN_FLG_SECURE;
    int flags_b = BN_FLG_SECURE;

    int neg_r = 0;
    int neg_a = 0;
    int neg_b = 0;

    int dmax_a = 4;
    int dmax_b = 4;
    int dmax_r = 4;

    int top_a = cal_top(a, dmax_a);
    int top_b = cal_top(b, dmax_b);
    int top_r = 0;

    //BN_ULONG *r = (BN_ULONG*)malloc(sizeof(BN_ULONG) * dmax_r);
    memset(r, 0, sizeof(BN_ULONG) * dmax_r);

    int max, min, dif;
    BN_ULONG t1, t2, borrow, * rp;
    const BN_ULONG* ap, * bp;

    bn_check_top(a);
    bn_check_top(b);

    max = top_a;
    min = top_b;
    dif = max - min;

    if (dif < 0) {              /* hmm... should not be happening */
        //ERR_raise(ERR_LIB_BN, BN_R_ARG2_LT_ARG3);
        printf("error\n");
        return 0;
    }

    r = bn_wexpand(r, top_r, flags_r, neg_r, &dmax_r, max);
    if (r == NULL/*bn_wexpand(r, max) == NULL*/)
        return 0;

    ap = a;
    bp = b;
    rp = r;

    borrow = bn_sub_words(rp, ap, bp, min);
    ap += min;
    rp += min;

    while (dif) {
        dif--;
        t1 = *(ap++);
        t2 = (t1 - borrow) & BN_MASK2;
        *(rp++) = t2;
        borrow &= (t1 == 0);
    }

    while (max && *--rp == 0)
        max--;

    top_r = max;
    neg_r = 0;
    bn_pollute(r);
    //memcpy(r, rr, sizeof(*r) * 4);
    memcpy(init_r, r, sizeof(BN_ULONG) * 4);

    return 1;
}




