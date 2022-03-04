#include "bn.h"

#  define LBITS(a)        ((a)&BN_MASK2l)
#  define HBITS(a)        (((a)>>BN_BITS4)&BN_MASK2l)
#  define L2HBITS(a)      (((a)<<BN_BITS4)&BN_MASK2)
#  define mul64(l,h,bl,bh) \
        { \
        BN_ULONG m,m1,lt,ht; \
 \
        lt=l; \
        ht=h; \
        m =(bh)*(lt); \
        lt=(bl)*(lt); \
        m1=(bl)*(ht); \
        ht =(bh)*(ht); \
        m=(m+m1)&BN_MASK2; if (m < m1) ht+=L2HBITS((BN_ULONG)1); \
        ht+=HBITS(m); \
        m1=L2HBITS(m); \
        lt=(lt+m1)&BN_MASK2; if (lt < m1) ht++; \
        (l)=lt; \
        (h)=ht; \
        }
#  define mul_add(r,a,bl,bh,c) { \
        BN_ULONG l,h; \
 \
        h= (a); \
        l=LBITS(h); \
        h=HBITS(h); \
        mul64(l,h,(bl),(bh)); \
 \
        /* non-multiply part */ \
        l=(l+(c))&BN_MASK2; if (l < (c)) h++; \
        (c)=(r); \
        l=(l+(c))&BN_MASK2; if (l < (c)) h++; \
        (c)=h&BN_MASK2; \
        (r)=l; \
        }

#  define mul(r,a,bl,bh,c) { \
        BN_ULONG l,h; \
 \
        h= (a); \
        l=LBITS(h); \
        h=HBITS(h); \
        mul64(l,h,(bl),(bh)); \
 \
        /* non-multiply part */ \
        l+=(c); if ((l&BN_MASK2) < (c)) h++; \
        (c)=h&BN_MASK2; \
        (r)=l&BN_MASK2; \
        }

#  define sqr64(lo,ho,in) \
        { \
        BN_ULONG l,h,m; \
 \
        h=(in); \
        l=LBITS(h); \
        h=HBITS(h); \
        m =(l)*(h); \
        l*=l; \
        h*=h; \
        h+=(m&BN_MASK2h1)>>(BN_BITS4-1); \
        m =(m&BN_MASK2l)<<(BN_BITS4+1); \
        l=(l+m)&BN_MASK2; if (l < m) h++; \
        (lo)=l; \
        (ho)=h; \
        }

BN_ULONG bn_add_words(BN_ULONG *r, const BN_ULONG *a, const BN_ULONG *b,
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

BN_ULONG bn_mul_add_words(BN_ULONG *rp, const BN_ULONG *ap, int num,
                          BN_ULONG w)
{
    BN_ULONG c = 0;
    BN_ULONG bl, bh;

    assert(num >= 0);
    if (num <= 0)
        return (BN_ULONG)0;

    bl = LBITS(w);
    bh = HBITS(w);

# ifndef OPENSSL_SMALL_FOOTPRINT
    while (num & ~3) {
        mul_add(rp[0], ap[0], bl, bh, c);
        mul_add(rp[1], ap[1], bl, bh, c);
        mul_add(rp[2], ap[2], bl, bh, c);
        mul_add(rp[3], ap[3], bl, bh, c);
        ap += 4;
        rp += 4;
        num -= 4;
    }
# endif
    while (num) {
        mul_add(rp[0], ap[0], bl, bh, c);
        ap++;
        rp++;
        num--;
    }
    return c;
}

BN_ULONG bn_mul_words(BN_ULONG *rp, const BN_ULONG *ap, int num, BN_ULONG w)
{
    BN_ULONG carry = 0;
    BN_ULONG bl, bh;

    assert(num >= 0);
    if (num <= 0)
        return (BN_ULONG)0;

    bl = LBITS(w);
    bh = HBITS(w);

# ifndef OPENSSL_SMALL_FOOTPRINT
    while (num & ~3) {
        mul(rp[0], ap[0], bl, bh, carry);
        mul(rp[1], ap[1], bl, bh, carry);
        mul(rp[2], ap[2], bl, bh, carry);
        mul(rp[3], ap[3], bl, bh, carry);
        ap += 4;
        rp += 4;
        num -= 4;
    }
# endif
    while (num) {
        mul(rp[0], ap[0], bl, bh, carry);
        ap++;
        rp++;
        num--;
    }
    return carry;
}

void bn_sqr_words(BN_ULONG *r, const BN_ULONG *a, int n)
{
    assert(n >= 0);
    if (n <= 0)
        return;

# ifndef OPENSSL_SMALL_FOOTPRINT
    while (n & ~3) {
        sqr64(r[0], r[1], a[0]);
        sqr64(r[2], r[3], a[1]);
        sqr64(r[4], r[5], a[2]);
        sqr64(r[6], r[7], a[3]);
        a += 4;
        r += 8;
        n -= 4;
    }
# endif
    while (n) {
        sqr64(r[0], r[1], a[0]);
        a++;
        r += 2;
        n--;
    }
}

/* tmp must have 2*n words */
void bn_sqr_normal(BN_ULONG *r, const BN_ULONG *a, int n, BN_ULONG *tmp)
{
    int i, j, max;
    const BN_ULONG *ap;
    BN_ULONG *rp;

    max = n * 2;
    ap = a;
    rp = r;
    rp[0] = rp[max - 1] = 0;
    rp++;
    j = n;

    if (--j > 0) {
        ap++;
        rp[j] = bn_mul_words(rp, ap, j, ap[-1]);
        rp += 2;
    }

    for (i = n - 2; i > 0; i--) {
        j--;
        ap++;
        rp[j] = bn_mul_add_words(rp, ap, j, ap[-1]);
        rp += 2;
    }

    bn_add_words(r, r, r, max);

    /* There will not be a carry */

    bn_sqr_words(tmp, a, n);

    bn_add_words(r, r, tmp, max);
}

int bn_sqr_fixed_top(BN_ULONG *r, const BN_ULONG *a, int* top_r, int* flags_r, int* neg_r)
{
    int flags_a = BN_FLG_SECURE;

    int neg_a = 0;


    int dmax_a = 4;
    int dmax_r = 4;

    int top_a = cal_top(a, dmax_a);

    int max, al;
    int ret = 0;
    BN_ULONG *tmp, *rr;

    bn_check_top(a);

    al = top_a;
    if (al <= 0) {
        *top_r = 0;
        *neg_r = 0;
        return 1;
    }

    rr = r;
    tmp = malloc(4 * sizeof(BN_ULONG));
    memset(tmp, 0, sizeof(BN_ULONG) * 4);

    // rr = (a != r) ? r : BN_CTX_get(ctx);
    // tmp = BN_CTX_get(ctx);
    // if (rr == NULL || tmp == NULL)
    //     goto err;

    max = 2 * al;               /* Non-zero (from above) */
    if (bn_wexpand(rr, *top_r, *flags_r, *neg_r, &dmax_r, max) == NULL)
        goto err;

    if (al == 4) {
        BN_ULONG t[8];
        bn_sqr_normal(rr, a, 4, t);
    } else if (al == 8) {
        BN_ULONG t[16];
        bn_sqr_normal(rr, a, 8, t);
    } else {
        if (bn_wexpand(tmp, *top_r, *flags_r, *neg_r, &dmax_r, max) == NULL)
            goto err;
        bn_sqr_normal(rr, a, al, tmp);
    }

    *neg_r = 0;
    *top_r = max;
    *flags_r |= BN_FLG_FIXED_TOP;
    memcpy(r, rr, sizeof(BN_ULONG) * 4);

    ret = 1;
 err:
    bn_check_top(rr);
    bn_check_top(tmp);
    return ret;
}

int raw_pow(const BN_ULONG *a, BN_ULONG *r)
{
    int flags_r = 0;
    int neg_r = 0;
    memset(r, 0, sizeof(BN_ULONG) * 4);
    int top_r = 0;
    int ret = bn_sqr_fixed_top(r, a, &top_r, &flags_r, &neg_r);

    bn_correct_top(r, &top_r, &flags_r, &neg_r);
    bn_check_top(r);

    return ret;
}


// uint64_t static_a[4];
// uint64_t static_result[8];

// int main()
// {
//   uint64_t *alloc_a = malloc(32);
//   uint64_t *alloc_result = malloc(64);

//   for (long long i = 0;i < 4;++i) {
//     unsigned long long x;
//     if (scanf("%llu",&x) != 1) abort();
//     static_a[i] = x;
//   }
//   for (long long i = 0;i < 8;++i) {
//     // unsigned long long x;
//     // if (scanf("%llu",&x) != 1) abort();
//     static_result[i] = 0;
//   }

//   for (long long i = 0;i < 4;++i)
//     alloc_a[i] = static_a[i];
//   for (long long i = 0;i < 8;++i)
//     alloc_result[i] = static_result[i];

//   raw_pow(alloc_a,alloc_result);

//   for (long long i = 0;i < 8;++i)
//     static_result[i] = alloc_result[i];

//   for (long long i = 0;i < 8;++i) {
//     unsigned long long x = static_result[i];
//     printf("%llu\n",x);
//   }

//   return 0;
// }