#include <stdint.h>
#include <immintrin.h>

void u32_add(uint64_t* r, const uint64_t* a, const uint64_t* b)
{
    memset(r, 0, sizeof(uint64_t) * 4);
    uint8_t carry = 0;
    for (size_t i = 0; i < 4; ++i)
        carry = _addcarryx_u64(carry, a[i], b[i], r + i);
}
