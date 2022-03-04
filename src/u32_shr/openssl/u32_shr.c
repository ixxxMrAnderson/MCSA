#include "bn.h"


int bn_rshift_fixed_top(BN_ULONG** r, int* top_r, int* flags_r, int* neg_r, int* dmax_r,
    const BN_ULONG* a, int top_a, int flags_a, int neg_a, int dmax_a,
    int n)
{
    int i, top, nw;
    unsigned int lb, rb;
    BN_ULONG* t, * f;
    BN_ULONG l, m, mask;

    bn_check_top(*r);
    bn_check_top(a);

    assert(n >= 0);

    nw = n / BN_BITS2;

    if (nw >= top_a) {
        /* shouldn't happen, but formally required */
        //BN_zero(r);
        BN_zero(top_r, flags_r, neg_r);
        return 1;
    }


    rb = (unsigned int)n % BN_BITS2;
    lb = BN_BITS2 - rb;
    lb %= BN_BITS2;            /* say no to undefined behaviour */
    mask = (BN_ULONG)0 - lb;   /* mask = 0 - (lb != 0) */
    mask |= mask >> 8;
    top = top_a - nw;


    *r = bn_wexpand(*r, *top_r, *flags_r, *neg_r, dmax_r, top);
    if (*r != a && /*bn_wexpand(r, top)*/*r == NULL)
        return 0;

    t = &((*r)[0]);
    f = &(a[nw]);
    l = f[0];
    for (i = 0; i < top - 1; i++) {
        m = f[i + 1];
        t[i] = (l >> rb) | ((m << lb) & mask);
        l = m;
    }
    t[i] = l >> rb;

    *neg_r = neg_a;
    *top_r = top;
    *flags_r |= BN_FLG_FIXED_TOP;

    return 1;
}


int u32_shr(BN_ULONG* r/*, *//*int* top_r, int* flags_r, int* neg_r, int* dmax_r,*/
    /*const BN_ULONG* a,*//* int top_a, int flags_a, int neg_a, int dmax_a,*/
    /*int n*/)
{
    int n = 1;
    int flags_r = 0; //BN_FLG_SECURE;
    int flags_a = BN_FLG_SECURE;

    int neg_r = 0;
    int neg_a = 0;

    int dmax_a = 4;
    int dmax_r = 4;

    int top_r = cal_top(r, dmax_r);
    int top_a = top_r;

    BN_ULONG* a = (BN_ULONG*)malloc(sizeof(BN_ULONG) * dmax_a);
    memcpy(a, r, sizeof(BN_ULONG) * dmax_a);

    int ret = 0;

    if (n < 0) {
        //ERR_raise(ERR_LIB_BN, BN_R_INVALID_SHIFT);
        printf("error BN_lshift\n");
        return 0;
    }

    /*ret = bn_rshift_fixed_top(r, &top_r, &flags_r, &neg_r, &dmax_r,
        a, top_a, flags_a, neg_a, dmax_a,
        n);*/
    ret = bn_rshift_fixed_top(&a, &top_a, &flags_a, &neg_a, &dmax_a,
        r, top_r, flags_r, neg_r, dmax_r,
        n);

    memcpy(r, a, sizeof(BN_ULONG) * 4);
    top_r = top_a;
    flags_r = flags_a;
    neg_r = neg_a;

    bn_correct_top(r, &top_r, &flags_r, &neg_r);
    bn_check_top(r);

    return ret;


}