#include "botan.h"
#include "utils.h"
#include "string.h"
#include "bits_ops.h"

// if arg is zero, return ~0, otherwise return 0
word is_zero(word a) {
    return ct_is_zero(a);
}

word expand(word a) {
    return ~is_zero(a);
}


void copy_mem(word* out, const word* in, size_t n)
{
    //static_assert(std::is_trivial<typename std::decay<T>::type>::value, "");
    //BOTAN_ASSERT_IMPLICATION(n > 0, in != nullptr && out != nullptr,
    //    "If n > 0 then args are not null");

    if (in != NULL && out != NULL && n > 0)
    {
        /*std::memmove*/
        memcpy(out, in, sizeof(word) * n);
    }
}

void clear_bytes(void* ptr, size_t bytes)
{
    if (bytes > 0)
    {
        memset(ptr, 0, bytes);
    }
}

void clear_mem(word* ptr, size_t n)
{
    clear_bytes(ptr, sizeof(word) * n);
}


size_t min(size_t a, size_t b) {
    if (a < b) {
        return a;
    }
    return b;
}

#define SZ 4
//template<size_t SZ>
int sized_for_comba_mul(size_t x_sw, size_t x_size,
    size_t y_sw, size_t y_size,
    size_t z_size)
{
    return (x_sw <= SZ && x_size >= SZ &&
        y_sw <= SZ && y_size >= SZ &&
        z_size >= 2 * SZ);
}

//template<size_t SZ>
int sized_for_comba_sqr(size_t x_sw, size_t x_size,
    size_t z_size)
{
    return (x_sw <= SZ && x_size >= SZ && z_size >= 2 * SZ);
}

/**
* Return a Mask<T> which is set if x == y
*/
/*static Mask<T>*/
word is_equal(word x, word y)
{
    return is_zero((word)(x ^ y));
}

/**
* Return a Mask<T> which is set if x < y
*/
/*static Mask<T>*/ 
word is_lt(word x, word y)
{
    return (expand_top_bit(x ^ ((x ^ y) | ((x - y) ^ x))));
}

/**
* Return a Mask<T> which is set if x > y
*/
/*static Mask<T>*/
word is_gt(word x, word y)
{
    return is_lt(y, x);
}

/**
* If this mask is set, return x, otherwise return y
*/
word select(word mask, word x, word y)
{
    return choose(mask, x, y);
}
