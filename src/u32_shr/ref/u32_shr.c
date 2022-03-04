#include <stdint.h>
#include <x86intrin.h>

void u32_shr(uint64_t* input)
{
#ifdef _MSC_VER
    input[0] = __shiftright128(input[0], input[1], 1);
    input[1] = __shiftright128(input[1], input[2], 1);
    input[2] = __shiftright128(input[2], input[3], 1);
#else
    input[0] = (input[1] << 63) | (input[0] >> 1);
    input[1] = (input[2] << 63) | (input[1] >> 1);
    input[2] = (input[3] << 63) | (input[2] >> 1);
#endif
    input[3] >>= 1;
}