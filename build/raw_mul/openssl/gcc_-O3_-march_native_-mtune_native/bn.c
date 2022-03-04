#include "bn.h"
void OPENSSL_secure_clear_free(BN_ULONG* a, size_t size) {
    memset(a, 0, size);
    free(a);
    a = NULL;
}

void OPENSSL_clear_free(BN_ULONG* a, size_t size) {
    memset(a, 0, size);
    free(a);
    a = NULL;
}

void OPENSSL_free(BN_ULONG* a) {
    free(a);
    a = NULL;
}


static void bn_free_d(BN_ULONG* a, int flags_a, int dmax_a,
    int clear)
{
    if (BN_get_flags(flags_a, BN_FLG_SECURE))
        OPENSSL_secure_clear_free(a, dmax_a * sizeof(a[0]));
    else if (clear != 0)
        OPENSSL_clear_free(a, dmax_a * sizeof(a[0]));
    else
        OPENSSL_free(a);
}

static BN_ULONG* bn_expand_internal(const BN_ULONG* b, int top_b, int flags_b, /*int neg_b,*/
    int words)
{
    BN_ULONG* a = NULL;

    if (words > (INT_MAX / (4 * BN_BITS2))) {
        //ERR_raise(ERR_LIB_BN, BN_R_BIGNUM_TOO_LONG);
        printf("err_raise\n");
        return NULL;
    }
    if (BN_get_flags(flags_b, BN_FLG_STATIC_DATA)) {
        //ERR_raise(ERR_LIB_BN, BN_R_EXPAND_ON_STATIC_BIGNUM_DATA);
        printf("err_raise\n");
        return NULL;
    }
    if (BN_get_flags(flags_b, BN_FLG_SECURE)) {
        a = OPENSSL_secure_zalloc(words * sizeof(*a));
        memset(a, 0, words * sizeof(*a));
    }
    else {
        a = OPENSSL_zalloc(words * sizeof(*a));
        memset(a, 0, words * sizeof(*a));
    } 
    if (a == NULL) {
        //ERR_raise(ERR_LIB_BN, ERR_R_MALLOC_FAILURE);
        printf("err_raise\n");
        return NULL;
    }
    assert(top_b <= words);
    if (top_b > 0)
        memcpy(a, b, sizeof(*a) * top_b);

    return a;
}


BN_ULONG* bn_expand2(BN_ULONG* b, int top_b, int flags_b, /*int neg_b,*/ int* dmax_b,
    /*BN_ULONG *r, int top_r, int flags_r, int neg_r, int dmax_r,*/
    int words)
{
    if (words > *dmax_b) {
        BN_ULONG* a = bn_expand_internal(b, top_b, flags_b, words);
        if (!a)
            return NULL;
        if (b != NULL)
            bn_free_d(b, flags_b, *dmax_b, 1);
        b = a;
        //b = NULL;
        *dmax_b = words;
    }

    //return 1;
    return b;
    //return b;
}


BN_ULONG* bn_wexpand(BN_ULONG* a, int top_a, int flags_a, int neg_a, int* dmax_a,
    /*BN_ULONG *r, int top_r, int flags_r, int neg_r, int dmax_r,*/
    int words)
{

    /*
        if(words <= dmax_a){
            r = a;
            top_r = top_a;
            flags_r = flags_a;
            neg_r = neg_a;
            dmax_r = dmax_a;
        }else{
            bn_expand2(a, top_a, flags_a, dmax_a, words);
            r = a;
            top_r = top_a;
            flags_r = flags_a;
            neg_r = neg_a;
            dmax_r = dmax_a;
        }
    */
    if (words > *dmax_a) {
        a = bn_expand2(a, top_a, flags_a, dmax_a, words);
    }
    //else {

    //    if (top_a == 0 && *dmax_a != 0) {
    //        memset(a, 0, dmax_a);
    //    }
    //}
    return a;
    /*
    r = a;
    top_r = top_a;
    flags_r = flags_a;
    neg_r = neg_a;
    dmax_r = dmax_a;
    */

    //return (words <= a->dmax) ? a : bn_expand2(a, words);

}


int cal_top(BN_ULONG* a, int dmax_a) {
    int top = dmax_a;
    for (int i = dmax_a-1; i >= 0; i--) {
        if (a[i] == 0) {
            top--;
        }
        else {
            break;
        }
    }
    return top;
}



//for bn lshift
void bn_correct_top(BN_ULONG* a, int* top_a, int* flags_a, int* neg_a)
{
    BN_ULONG* ftl;
    int tmp_top = *top_a;

    if (tmp_top > 0) {
        for (ftl = &(a[tmp_top]); tmp_top > 0; tmp_top--) {
            ftl--;
            if (*ftl != 0)
                break;
        }
        *top_a = tmp_top;
    }
    if (*top_a == 0)
        *neg_a = 0;
    *flags_a &= ~BN_FLG_FIXED_TOP;
    bn_pollute(a);
}

//for bn rshift
void BN_zero_ex(/*BN_ULONG* a,*/int* top_a, int* flags_a, int* neg_a)
{
    *neg_a = 0;
    *top_a = 0;
    *flags_a &= ~BN_FLG_FIXED_TOP;
}



