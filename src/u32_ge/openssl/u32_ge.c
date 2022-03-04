#include "bn.h"

uint32_t u32_ge(const BN_ULONG* a, const BN_ULONG* b)
{
    int i;
    BN_ULONG t1, t2;
    int dmax_a = 4;
    int dmax_b = 4;

    for (i = dmax_a - 1; i >= 0; i--) {
        t1 = a[i];
        t2 = b[i];
        if (t1 != t2)
            return ((t1 > t2) ? 1 : -1);
    }
    return 0;
}