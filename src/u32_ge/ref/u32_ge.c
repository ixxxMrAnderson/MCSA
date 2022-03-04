#include <stdint.h>

uint32_t u32_ge(uint64_t* a, uint64_t* b)
{
	if (a[3] != b[3])
		return (a[3] > b[3]);
	if (a[2] != b[2])
		return (a[2] > b[2]);
	if (a[1] != b[1])
		return (a[1] > b[1]);
	return (a[0] >= b[0]);
}