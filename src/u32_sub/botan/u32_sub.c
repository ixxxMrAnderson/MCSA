#include "botan.h"
/**
* Three operand subtraction
*/
word bigint_sub3(word z[],
                        const word x[], size_t x_size,
                        const word y[], size_t y_size)
   {
   word borrow = 0;

   //BOTAN_ASSERT(x_size >= y_size, "Expected sizes");

   const size_t blocks = y_size - (y_size % 8);

   for(size_t i = 0; i != blocks; i += 8)
      borrow = word8_sub3(z + i, x + i, y + i, borrow);

   for(size_t i = blocks; i != y_size; ++i)
      //z[i] = word_sub(x[i], y[i], &borrow);
       word_sub(x[i], y[i], &borrow,z+i);

   for(size_t i = y_size; i != x_size; ++i)
      //z[i] = word_sub(x[i], 0, &borrow);
       word_sub(x[i], y[i], &borrow, z + i);

   return borrow;
   }

void u32_sub(word r[4], const word a[4], const word b[4]){

   bigint_sub3(r,a,4,b,4);

}