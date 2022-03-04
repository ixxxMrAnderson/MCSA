#include "botan.h"
/**
* If top bit of arg is set, return ~0. Otherwise return 0.
*/
//template<typename T>
word expand_top_bit(word a)
{
	return (word)(0) - (a >> (sizeof(word) * 8 - 1));
}

/**
* If arg is zero, return ~0. Otherwise return 0
*/
//template<typename T>
word ct_is_zero(word x)
{
	return expand_top_bit(~x & (x - 1));
}


/*template<typename T>
inline constexpr T*/
word choose(word mask, word a, word b)
{
	//return (mask & a) | (~mask & b);
	return (b ^ (mask & (a ^ b)));
}