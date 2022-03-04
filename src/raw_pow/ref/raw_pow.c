#include <stdint.h>
#include <immintrin.h>
#include <string.h>
#include <x86intrin.h>

// void raw_pow(uint64_t a[4], uint64_t result[8]);


void raw_pow(const uint64_t* x, uint64_t result[8])
{
	uint64_t h, l;
	uint8_t carry, carry2 = 0;
	result[0] = result[1] = result[2] = 0;
#define power(a) \
	l = _mulx_u64(x[a], x[a], &h); \
	carry = _addcarryx_u64(0, result[a*2], l, result+a*2); \
	carry2 += _addcarryx_u64(carry, result[a*2+1], h, result+a*2+1)
#define multiple2(a, b) \
	l = _mulx_u64(x[a], x[b], &h); \
	carry = _addcarryx_u64(0, result[a+b], (l<<1), result+a+b); \
	carry2 += _addcarryx_u64(carry, result[a+b+1], (h<<1)|(l>>63), result+a+b+1) + (h>>63)
	power(0);  // carry2 here must remains 0.
	multiple2(0, 1);
	result[3] = carry2; carry2 = 0;
	power(1); multiple2(0, 2);
	result[4] = carry2; carry2 = 0;
	multiple2(0, 3); multiple2(1, 2);
	result[5] = carry2; carry2 = 0;
	power(2); multiple2(1, 3);
	result[6] = carry2; carry2 = 0;
	multiple2(2, 3);
	result[7] = carry2; // carry2 comes useless
	power(3);
#undef power
#undef multiple2
}

// uint64_t static_a[4];
// uint64_t static_result[8];

// int main()
// {
//   uint64_t *alloc_a = malloc(32);
//   uint64_t *alloc_result = malloc(64);

//   for (long long i = 0;i < 4;++i) {
//     unsigned long long x;
//     if (scanf("%llu",&x) != 1) abort();
//     static_a[i] = x;
//   }
//   for (long long i = 0;i < 8;++i) {
//     // unsigned long long x;
//     // if (scanf("%llu",&x) != 1) abort();
//     static_result[i] = 0;
//   }

//   for (long long i = 0;i < 4;++i)
//     alloc_a[i] = static_a[i];
//   for (long long i = 0;i < 8;++i)
//     alloc_result[i] = static_result[i];

//   raw_pow(alloc_a,alloc_result);

//   for (long long i = 0;i < 8;++i)
//     static_result[i] = alloc_result[i];

//   for (long long i = 0;i < 8;++i) {
//     unsigned long long x = static_result[i];
//     printf("%llu\n",x);
//   }

//   return 0;
// }