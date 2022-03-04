#include "bn.h"


BN_ULONG bn_add_words(BN_ULONG* r, const BN_ULONG* a, const BN_ULONG* b,
    int n)
{
    BN_ULONG c, l, t;

    assert(n >= 0);
    if (n <= 0)
        return (BN_ULONG)0;

    c = 0;
# ifndef OPENSSL_SMALL_FOOTPRINT
    while (n & ~3) {
        t = a[0];
        t = (t + c) & BN_MASK2;
        c = (t < c);
        l = (t + b[0]) & BN_MASK2;
        c += (l < t);
        r[0] = l;
        t = a[1];
        t = (t + c) & BN_MASK2;
        c = (t < c);
        l = (t + b[1]) & BN_MASK2;
        c += (l < t);
        r[1] = l;
        t = a[2];
        t = (t + c) & BN_MASK2;
        c = (t < c);
        l = (t + b[2]) & BN_MASK2;
        c += (l < t);
        r[2] = l;
        t = a[3];
        t = (t + c) & BN_MASK2;
        c = (t < c);
        l = (t + b[3]) & BN_MASK2;
        c += (l < t);
        r[3] = l;
        a += 4;
        b += 4;
        r += 4;
        n -= 4;
    }
# endif
    while (n) {
        t = a[0];
        t = (t + c) & BN_MASK2;
        c = (t < c);
        l = (t + b[0]) & BN_MASK2;
        c += (l < t);
        r[0] = l;
        a++;
        b++;
        r++;
        n--;
    }
    return (BN_ULONG)c;
}


int u32_add(BN_ULONG* r, const BN_ULONG* a, const BN_ULONG* b)
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

    memset(r, 0, sizeof(BN_ULONG) * dmax_r);


    int top_a = cal_top(a, dmax_a);
    int top_b = cal_top(b, dmax_b);
    int top_r = 0;



    int max, min, dif;
    const BN_ULONG* ap, * bp;
    BN_ULONG* rp, carry, t1, t2;

    if (a != NULL) {
        bn_check_top(top_a);
    }
    if (b != NULL) {
        bn_check_top(top_b);
    }
    if (top_a < top_b) {
        const BN_ULONG* tmp;

        tmp = a;
        a = b;
        b = tmp;

        int tmp_flags;
        int tmp_top;
        int tmp_neg;

        tmp_flags = flags_a;
        flags_a = flags_b;
        flags_b = tmp_flags;

        tmp_top = top_a;
        top_a = top_b;
        top_b = tmp_top;

        tmp_neg = neg_a;
        neg_a = neg_b;
        neg_b = tmp_neg;
    }
    max = top_a;
    min = top_b;
    dif = max - min;
    r = bn_wexpand(r, top_r, flags_r, neg_r, &dmax_r, max + 1);
    if (r == NULL)
        return NULL;

    top_r = max;

    ap = a;
    bp = b;
    rp = r;

    carry = bn_add_words(rp, ap, bp, min);
    rp += min;
    ap += min;

    while (dif) {
        dif--;
        t1 = *(ap++);
        t2 = (t1 + carry) & BN_MASK2;
        *(rp++) = t2;
        carry &= (t2 == 0);
    }
    *rp = carry;
    top_r += carry;

    neg_r = 0;
    bn_check_top(r);
    memcpy(init_r, r, sizeof(BN_ULONG) * 4);

    return 1;
}