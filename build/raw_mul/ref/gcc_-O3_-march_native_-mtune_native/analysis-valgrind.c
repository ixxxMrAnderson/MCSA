#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

extern void word3_muladd(uint64_t *,uint64_t *,uint64_t *,uint64_t,uint64_t);

uint64_t static_x[1];
uint64_t static_y[1];
uint64_t static_z[1];
uint64_t static_a[1];
uint64_t static_b[1];

int main(int argc,char **argv)
{
  uint64_t *alloc_x = malloc(8);
  uint64_t *alloc_y = malloc(8);
  uint64_t *alloc_z = malloc(8);
  uint64_t *alloc_a = malloc(8);
  uint64_t *alloc_b = malloc(8);

  word3_muladd(alloc_x,alloc_y,alloc_z,*alloc_a,*alloc_b);

  return 0;
}
