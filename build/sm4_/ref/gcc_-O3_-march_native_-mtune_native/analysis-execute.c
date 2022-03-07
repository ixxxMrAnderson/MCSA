#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

extern uint32_t SM4_T(uint32_t);

uint32_t static_x[1];
uint32_t static_z[1];

int main(int argc,char **argv)
{
  uint32_t *alloc_x = malloc(4);
  uint32_t *alloc_z = malloc(4);

  for (long long i = 0;i < 1;++i) {
    unsigned long long x;
    if (scanf("%llu",&x) != 1) abort();
    static_x[i] = x;
  }

  for (long long i = 0;i < 1;++i)
    alloc_x[i] = static_x[i];

  alloc_z[0] = SM4_T(*alloc_x);

  for (long long i = 0;i < 1;++i)
    static_z[i] = alloc_z[i];

  for (long long i = 0;i < 1;++i) {
    unsigned long long x = static_z[i];
    printf("%llu\n",x);
  }
  fflush(stdout);

  return 0;
}
