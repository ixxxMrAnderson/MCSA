#include "botan.h"

uint8_t u32_eq(const word* a, const word* b){
	return (uint8_t)(cmp(a, b) == 0);
}