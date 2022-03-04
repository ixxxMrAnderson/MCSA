#include "bn.h"


int bn_lshift_fixed_top(BN_ULONG** r, int* top_r, int* flags_r, int* neg_r, int* dmax_r,
    const BN_ULONG* a, int top_a, int flags_a, int neg_a, int dmax_a,
    int n)
{
    int i, nw;
    unsigned int lb, rb;
    BN_ULONG* t, * f;
    BN_ULONG l, m, rmask = 0;

    assert(n >= 0);

    bn_check_top(*r);
    bn_check_top(a);

    nw = n / BN_BITS2;
    *r = bn_wexpand(*r, *top_r, *flags_r, *neg_r, dmax_r, top_a + nw + 1);
    if (*r == NULL/*bn_wexpand(r, a->top + nw + 1) == NULL*/)
        return 0;

    if (top_a != 0) {
        lb = (unsigned int)n % BN_BITS2;
        rb = BN_BITS2 - lb;
        rb %= BN_BITS2;            /* say no to undefined behaviour */
        rmask = (BN_ULONG)0 - rb;  /* rmask = 0 - (rb != 0) */
        rmask |= rmask >> 8;
        f = &(a[0]);
        t = &((*r)[nw]);
        l = f[top_a - 1];
        t[top_a] = (l >> rb) & rmask;
        for (i = top_a - 1; i > 0; i--) {
            m = l << lb;
            l = f[i - 1];
            t[i] = (m | ((l >> rb) & rmask)) & BN_MASK2;
        }
        t[0] = (l << lb) & BN_MASK2;
    }
    else {
        /* shouldn't happen, but formally required */
        (*r)[nw] = 0;
    }
    if (nw != 0)
        memset(*r, 0, sizeof(*t) * nw);

    *neg_r = neg_a;
    *top_r = top_a + nw + 1;
    *flags_r |= BN_FLG_FIXED_TOP;

    return 1;
}


int u32_shl(BN_ULONG* r)
{

    int n = 1;
    int flags_r = BN_FLG_SECURE;
    int flags_a = BN_FLG_SECURE;

    int neg_r = 0;
    int neg_a = 0;

    int dmax_a = 4;
    int dmax_r = 4;

    //int top_a = cal_top(a, dmax_a);
    int top_r = cal_top(r, dmax_r);
    int top_a = top_r;

    BN_ULONG* a = (BN_ULONG*)malloc(sizeof(BN_ULONG) * dmax_a);
    memcpy(a, r, sizeof(BN_ULONG) * dmax_a);
    //int ret = 0;

    if (n < 0) {
        //ERR_raise(ERR_LIB_BN, BN_R_INVALID_SHIFT);
        printf("error BN_lshift\n");
        return 0;
    }

    /*ret =*/bn_lshift_fixed_top(&a, &top_a, &flags_a, &neg_a, &dmax_a,
        r, top_r, flags_r, neg_r, dmax_r,
        n);

    memcpy(r, a, sizeof(*a) * 4);
    top_r = top_a;
    flags_r = flags_a;
    neg_r = neg_a;


    int carry = 0;
    if (dmax_a > 4) {
        carry = *(a + 4);
    }
    
    bn_correct_top(r, &top_r, &flags_r, &neg_r);
    bn_check_top(r);

    return carry;
}

