#include "botan.h"
#include "mp_asmi.h"
#include "bits_ops.h"
#include "utils.h"


/*

namespace botan{

    namespace CT {

        template<typename T>
        class Mask
        {
        public:
            //......

            //
            // Return a Mask<T> which is set if v is != 0
            //
            static Mask<T> expand(T v)
            {
                return ~Mask<T>::is_zero(v);
            }

            //
            // Return a Mask<T> which is set if v is == 0 or cleared otherwise
            //
            static Mask<T> is_zero(T x)
            {
                return Mask<T>(ct_is_zero<T>(x));
            }

             //
             // Return x if the mask is set, or otherwise zero
             //
            T if_set_return(T x) const
            {
                return m_mask & x;
            }

        private:
            Mask(T m) : m_mask(m) {}

            T m_mask;

        }
    }



    //
    // If top bit of arg is set, return ~0. Otherwise return 0.
    //
    template<typename T>
    inline constexpr T expand_top_bit(T a)
    {
        return static_cast<T>(0) - (a >> (sizeof(T) * 8 - 1));
    }

    //
    // If arg is zero, return ~0. Otherwise return 0
    //
    template<typename T>
    inline constexpr T ct_is_zero(T x)
    {
        return expand_top_bit<T>(~x & (x - 1));
    }

}
*/




void bigint_shl1(word x[], size_t x_size, size_t x_words,
    size_t word_shift, size_t bit_shift)
{
    //输入的例子中 word_shift 一定为0
    copy_mem(x + word_shift, x, x_words); //先左移 word 
    clear_mem(x, word_shift); // 低位置零

    //下面继续左移剩余不到1 word的bit
    //const auto carry_mask = CT::Mask<word>::expand(bit_shift); bit_shift is not zero, so mask set.
    //const size_t carry_shift = carry_mask.if_set_return(BOTAN_MP_WORD_BITS - bit_shift); id mask set, carry_shift = BOTAN_MP_WORD_BITS - bit_shift

    const word carry_mask = expand(bit_shift);
    const size_t carry_shift = carry_mask & (BOTAN_MP_WORD_BITS - bit_shift);

    word carry = 0;
    for (size_t i = word_shift; i != x_size; ++i)
    {
        const word w = x[i];
        x[i] = (w << bit_shift) | carry;
        //carry = carry_mask.if_set_return(w >> carry_shift);
        carry = carry_mask & (w >> carry_shift);
    }
}

void u32_shl(word a[4]) {
    bigint_shl1(a, 4, 4, 0, 1);
}