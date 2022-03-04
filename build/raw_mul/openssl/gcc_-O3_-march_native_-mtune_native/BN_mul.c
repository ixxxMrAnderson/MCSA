#include "bn.h"

void word3_muladd(BN_ULONG* c2, BN_ULONG* c1, BN_ULONG* c0, BN_ULONG a, BN_ULONG b) {
    mul_add_c(a, b, c0, c1, c2);
}

void mul_add_c(BN_ULONG a, BN_ULONG b, BN_ULONG* c0, BN_ULONG* c1, BN_ULONG* c2) {
    BN_ULONG lo, hi;                  
    __int128_t ret=(__int128_t)(a)*(b);
    hi = ret >> 64; 
    lo = ret;        
    *c0 += lo; hi += (*c0 < lo) ? 1 : 0;
    *c1 += hi; *c2 += (*c1 < hi) ? 1 : 0;
}

void bn_mul_comba4(BN_ULONG* r, BN_ULONG* a, BN_ULONG* b)
{
    BN_ULONG*c1, *c2, *c3;
    BN_ULONG cc1, cc2, cc3;

    cc1 = 0;
    cc2 = 0;
    cc3 = 0;

    c1 = &cc1;
    c2 = &cc2;
    c3 = &cc3;

    mul_add_c(a[0], b[0], c1, c2, c3);
    r[0] = *c1;
    *c1 = 0;
    mul_add_c(a[0], b[1], c2, c3, c1);
    mul_add_c(a[1], b[0], c2, c3, c1);
    r[1] = *c2;
    *c2 = 0;
    mul_add_c(a[2], b[0], c3, c1, c2);
    mul_add_c(a[1], b[1], c3, c1, c2);
    mul_add_c(a[0], b[2], c3, c1, c2);
    r[2] = *c3;
    *c3 = 0;
    // mul_add_c(a[0], b[3], c1, c2, c3);
    // mul_add_c(a[1], b[2], c1, c2, c3);
    // mul_add_c(a[2], b[1], c1, c2, c3);
    // mul_add_c(a[3], b[0], c1, c2, c3);
    *c1=*c2=*c3=0;
    r[3] = *c1;
    *c1 = 0;
    // mul_add_c(a[3], b[1], c2, c3, c1);
    // mul_add_c(a[2], b[2], c2, c3, c1);
    // mul_add_c(a[1], b[3], c2, c3, c1);
    r[4] = *c2;
    *c2 = 0;
    // mul_add_c(a[2], b[3], c3, c1, c2);
    // mul_add_c(a[3], b[2], c3, c1, c2);
    r[5] = *c3;
    // *c3 = 0;
    // mul_add_c(a[3], b[3], c1, c2, c3);
    r[6] = *c1;
    r[7] = *c2;
}




#define BN_MUL_COMBA
int bn_mul_fixed_top(BN_ULONG* r, BN_ULONG* a, BN_ULONG* b, int* top_r, int* flags_r, int* neg_r)
{
    BN_ULONG* init_r = r;

    int flags_a = BN_FLG_SECURE;
    int flags_b = BN_FLG_SECURE;

    int neg_a = 0;
    int neg_b = 0;


    int dmax_a = 4;
    int dmax_b = 4;
    int dmax_r = 8;


    int top_a = 4;
    int top_b = 4;

    //printf("%d%d\n", top_a, top_b);

    int ret = 0;
    int top;// , al, bl;

    bn_check_top(a);
    bn_check_top(b);
    bn_check_top(r);

    if ((top_a == 0) || (top_b == 0)) {
        BN_zero_ex(top_r, flags_r, neg_r);
        return 1;
    }
    top = top_a + top_b;




#if defined(BN_MUL_COMBA) || defined(BN_RECURSION)
    int i = top_a - top_b; //al - bl;
#endif
#ifdef BN_MUL_COMBA
    if (i == 0) {
        if (top_a == 4) {
            if (bn_wexpand(r, *top_r, *flags_r, *neg_r, &dmax_r, 8) == NULL)
                goto err;
            //rr->top = 8;
            *top_r = 8;
            bn_mul_comba4(r, a, b);
            goto end;
        }
    }
#endif  


#if defined(BN_MUL_COMBA) || defined(BN_RECURSION)
    end:
#endif

    *neg_r = neg_a ^ neg_b;
    *flags_r |= BN_FLG_FIXED_TOP;
    memcpy(init_r, r, sizeof(BN_ULONG) * 4);
    ret = 1;
err:
    bn_check_top(r);
    return ret;
}

int raw_mul(BN_ULONG* a, BN_ULONG* b, BN_ULONG* r)
{
    int flags_r = 0;
    int neg_r = 0;
    memset(r, 0, sizeof(BN_ULONG) * 8);
    int top_r = 0;

    int ret = bn_mul_fixed_top(r, a, b, &top_r, &flags_r, &neg_r);

    // bn_correct_top(r, &top_r, &flags_r, &neg_r);
    bn_check_top(r);

    return ret;
}
