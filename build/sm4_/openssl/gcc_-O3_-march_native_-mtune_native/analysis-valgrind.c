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

  alloc_z[0] = SM4_T(*alloc_x);

  return 0;
}
