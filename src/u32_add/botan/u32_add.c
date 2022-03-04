#include "botan.h"
#include "mp_asmi.h"

/**
* Three operand addition with carry out
*/
word bigint_add3_nc(word z[],
    const word x[], size_t x_size,
    const word y[], size_t y_size)
{
    if (x_size < y_size)
    {
        return bigint_add3_nc(z, y, y_size, x, x_size);
    }

    word carry = 0;

    const size_t blocks = y_size - (y_size % 8);

    for (size_t i = 0; i != blocks; i += 8)
        carry = word8_add3(z + i, x + i, y + i, carry);

    for (size_t i = blocks; i != y_size; ++i)
        z[i] = word_add(x[i], y[i], &carry);

    for (size_t i = y_size; i != x_size; ++i)
        z[i] = word_add(x[i], 0, &carry);

    return carry;
}

void bigint_add3(word z[],
    const word x[], size_t x_size,
    const word y[], size_t y_size)
{
    z[x_size > y_size ? x_size : y_size] +=
        bigint_add3_nc(z, x, x_size, y, y_size);
}

void u32_add(word z[4], const word x[4], const word y[4]) {
    bigint_add3(z, x, 4, y, 4);
}


/**
* Two operand addition with carry out
*/
/*inline*/ /*word bigint_add2_nc(word x[], size_t x_size, const word y[], size_t y_size)
{
    word carry = 0;

    //BOTAN_ASSERT(x_size >= y_size, "Expected sizes");

    const size_t blocks = y_size - (y_size % 8);

    for (size_t i = 0; i != blocks; i += 8)
        carry = word8_add2(x + i, y + i, carry);

    for (size_t i = blocks; i != y_size; ++i)
        x[i] = word_add(x[i], y[i], &carry);

    for (size_t i = y_size; i != x_size; ++i)
        x[i] = word_add(x[i], 0, &carry);

    return carry;
}
*/
/**
* Two operand addition
* @param x the first operand (and output)
* @param x_size size of x
* @param y the second operand
* @param y_size size of y (must be >= x_size)
*/
/*inline*/ /*void bigint_add2(word x[], size_t x_size,
    const word y[], size_t y_size)
{
    x[x_size] += bigint_add2_nc(x, x_size, y, y_size);
}
*/

