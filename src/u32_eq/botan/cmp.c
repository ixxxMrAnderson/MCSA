#include "botan.h"
#include "utils.h"

/**
* Compare x and y
* Return -1 if x < y
* Return 0 if x == y
* Return 1 if x > y
*/
inline int32_t bigint_cmp(const word x[], size_t x_size,
    const word y[], size_t y_size)
{
    //static_assert(sizeof(word) >= sizeof(uint32_t), "Size assumption");

    const word LT = (word)(-1);/*static_cast<word>(-1);*/
    const word EQ = 0;
    const word GT = 1;

    const size_t common_elems = min(x_size, y_size);

    word result = EQ; // until found otherwise

    for (size_t i = 0; i != common_elems; i++)
    {
        const word _is_eq = is_equal(x[i], y[i]); //本身是个mask
        const word _is_lt = is_lt(x[i], y[i]); // 本身是个mask

        result = select(_is_eq, result, select(_is_lt, LT, GT));
    }

    if (x_size < y_size)
    {
        word mask = 0;
        for (size_t i = x_size; i != y_size; i++)
            mask |= y[i];

        // If any bits were set in high part of y, then x < y
        result = select(is_zero(mask), result, LT);
    }
    else if (y_size < x_size)
    {
        word mask = 0;
        for (size_t i = y_size; i != x_size; i++)
            mask |= x[i];

        // If any bits were set in high part of x, then x > y
        result = select(is_zero(mask), result, GT);
    }

    //CT::unpoison(result);
    //BOTAN_DEBUG_ASSERT(result == LT || result == GT || result == EQ);
    return (int32_t)(result);
}

/*
* Comparison Function
*/
int32_t cmp_core(const word* data, const word* other, int check_signs)
{
    if (check_signs)
    {
        /*if (other.is_positive() && this->is_negative())
            return -1;

        if (other.is_negative() && this->is_positive())
            return 1;

        if (other.is_negative() && this->is_negative())
            return (-bigint_cmp(this->data(), this->size(),
                other.data(), other.size()));
                */
    }

    return bigint_cmp(data, 4, other, 4);
}

/**
* Compare x and y
* Return -1 if x < y
* Return 0 if x == y
* Return 1 if x > y
*/
int32_t cmp(const word* x, const word* y) {
    return cmp_core(x, y, 0);
}