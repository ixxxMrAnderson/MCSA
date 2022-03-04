#include "botan.h"
#include "utils.h"
#include "mp_asmi.h"


/*
* Comba 4x4 Squaring
*/
void bigint_comba_sqr4(word z[8], const word x[4])
{
	word w2 = 0, w1 = 0, w0 = 0;

	word3_muladd(&w2, &w1, &w0, x[0], x[0]);
	z[0] = w0; w0 = 0;

	word3_muladd_2(&w0, &w2, &w1, x[0], x[1]);
	z[1] = w1; w1 = 0;

	word3_muladd_2(&w1, &w0, &w2, x[0], x[2]);
	word3_muladd(&w1, &w0, &w2, x[1], x[1]);
	z[2] = w2; w2 = 0;

	word3_muladd_2(&w2, &w1, &w0, x[0], x[3]);
	word3_muladd_2(&w2, &w1, &w0, x[1], x[2]);
	z[3] = w0; w0 = 0;

	word3_muladd_2(&w0, &w2, &w1, x[1], x[3]);
	word3_muladd(&w0, &w2, &w1, x[2], x[2]);
	z[4] = w1; w1 = 0;

	word3_muladd_2(&w1, &w0, &w2, x[2], x[3]);
	z[5] = w2; w2 = 0;

	word3_muladd(&w2, &w1, &w0, x[3], x[3]);
	z[6] = w0;
	z[7] = w1;
}


/*
* Squaring Algorithm Dispatcher
*/
void bigint_sqr(word z[], size_t z_size,
    const word x[], size_t x_size, size_t x_sw/*,
    word workspace[], size_t ws_size*/)
{
    clear_mem(z, z_size);

    //BOTAN_ASSERT(z_size / 2 >= x_sw, "Output size is sufficient");

    if (x_sw == 1)
    {
        //bigint_linmul3(z, x, x_sw, x[0]);
    }
    else if (sized_for_comba_sqr(x_sw, x_size, z_size))
    {
        bigint_comba_sqr4(z, x);
    }
    /*else if (sized_for_comba_sqr<6>(x_sw, x_size, z_size))
    {
        bigint_comba_sqr6(z, x);
    }
    else if (sized_for_comba_sqr<8>(x_sw, x_size, z_size))
    {
        bigint_comba_sqr8(z, x);
    }
    else if (sized_for_comba_sqr<9>(x_sw, x_size, z_size))
    {
        bigint_comba_sqr9(z, x);
    }
    else if (sized_for_comba_sqr<16>(x_sw, x_size, z_size))
    {
        bigint_comba_sqr16(z, x);
    }
    else if (sized_for_comba_sqr<24>(x_sw, x_size, z_size))
    {
        bigint_comba_sqr24(z, x);
    }
    else if (x_size < KARATSUBA_SQUARE_THRESHOLD || !workspace)
    {
        basecase_sqr(z, z_size, x, x_sw);
    }
    else
    {
        const size_t N = karatsuba_size(z_size, x_size, x_sw);

        if (N && z_size >= 2 * N && ws_size >= 2 * N)
            karatsuba_sqr(z, x, N, workspace);
        else
            basecase_sqr(z, z_size, x, x_sw);
    }*/
    else {
        printf("Other situation.");
    }
}


void raw_pow(word a[4], word r[8]) {
    // bigint_sqr(r, 8,
    //     a, 4, 4);
}