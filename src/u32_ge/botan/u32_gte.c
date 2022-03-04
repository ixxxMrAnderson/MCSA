#include "botan.h"

uint8_t u32_ge(const word* a, const word* b) {
	int32_t res = cmp(a, b);
	return (uint8_t)(res == 0 || res == 1);
}