#include <stdint.h>
#include <x86intrin.h>

uint8_t u32_shl(uint64_t* input)
{
    uint8_t carry = (input[3] >> 63);
#ifdef _MSC_VER
    input[3] = __shiftleft128(input[2], input[3], 1);
		input[2] = __shiftleft128(input->v[1], input[2], 1);
		input[1] = __shiftleft128(input[0], input[1], 1);
#else
    input[3] = (input[3] << 1) | (input[2] >> 63);
    input[2] = (input[2] << 1) | (input[1] >> 63);
    input[1] = (input[1] << 1) | (input[0] >> 63);
#endif
    input[0] <<= 1;
    return carry;
}