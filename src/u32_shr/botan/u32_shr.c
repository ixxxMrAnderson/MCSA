#include "botan.h"
#include "mp_asmi.h"
#include "bits_ops.h"
#include "utils.h"


void bigint_shr1(word x[], size_t x_size,
    size_t word_shift, size_t bit_shift)
{
    const size_t top = x_size >= word_shift ? (x_size - word_shift) : 0;

    if (top > 0)
        copy_mem(x, x + word_shift, top);
    clear_mem(x + top, min(word_shift, x_size));

    const word carry_mask = expand(bit_shift);
    const size_t carry_shift = carry_mask & (BOTAN_MP_WORD_BITS - bit_shift);

    word carry = 0;

    for (size_t i = 0; i != top; ++i)
    {
        const word w = x[top - i - 1];
        x[top - i - 1] = (w >> bit_shift) | carry;
        carry = carry_mask & (w << carry_shift);
        //carry = carry_mask.if_set_return(w << carry_shift);
    }
}

void u32_shr(word a[4]) {
    bigint_shr1(a, 4, 0, 1);
}