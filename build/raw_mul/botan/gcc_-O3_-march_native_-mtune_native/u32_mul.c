#include "botan.h"
#include "utils.h"
#include "mp_asmi.h"


/*
* Comba 4x4 Multiplication
*/
void bigint_comba_mul4(word z[8], const word x[4], const word y[4])
{
    word w2 = 0, w1 = 0, w0 = 0;

    word3_muladd(&w2, &w1, &w0, x[0], y[0]);
    z[0] = w0; w0 = 0;

    word3_muladd(&w0, &w2, &w1, x[0], y[1]);
    word3_muladd(&w0, &w2, &w1, x[1], y[0]);
    z[1] = w1; w1 = 0;

    word3_muladd(&w1, &w0, &w2, x[0], y[2]);
    word3_muladd(&w1, &w0, &w2, x[1], y[1]);
    word3_muladd(&w1, &w0, &w2, x[2], y[0]);
    z[2] = w2; w2 = 0;

    // word3_muladd(&w2, &w1, &w0, x[0], y[3]);
    // word3_muladd(&w2, &w1, &w0, x[1], y[2]);
    // word3_muladd(&w2, &w1, &w0, x[2], y[1]);
    // word3_muladd(&w2, &w1, &w0, x[3], y[0]);
    w0=w1=w2=0;
    z[3] = w0; 
    w0 = 0;

    // word3_muladd(&w0, &w2, &w1, x[1], y[3]);
    // word3_muladd(&w0, &w2, &w1, x[2], y[2]);
    // word3_muladd(&w0, &w2, &w1, x[3], y[1]);
    z[4] = w1; w1 = 0;

    // word3_muladd(&w1, &w0, &w2, x[2], y[3]);
    // word3_muladd(&w1, &w0, &w2, x[3], y[2]);
    z[5] = w2; w2 = 0;

    // word3_muladd(&w2, &w1, &w0, x[3], y[3]);
    z[6] = w0;
    z[7] = w1;
}

void bigint_mul(word z[], size_t z_size,
    const word x[], size_t x_size, size_t x_sw,
    const word y[], size_t y_size, size_t y_sw/*,
    word workspace[], size_t ws_size*/)
{
    clear_mem(z, z_size);

    if (x_sw == 1)
    {
        //bigint_linmul3(z, y, y_sw, x[0]);
    }
    else if (y_sw == 1)
    {
        //bigint_linmul3(z, x, x_sw, y[0]);
    }
    else if (sized_for_comba_mul(x_sw, x_size, y_sw, y_size, z_size))
    {
        bigint_comba_mul4(z, x, y);
    }
    /*else if (sized_for_comba_mul(x_sw, x_size, y_sw, y_size, z_size))
    {
        //bigint_comba_mul6(z, x, y);
    }
    else if (sized_for_comba_mul<8>(x_sw, x_size, y_sw, y_size, z_size))
    {
        //bigint_comba_mul8(z, x, y);
    }
    else if (sized_for_comba_mul<9>(x_sw, x_size, y_sw, y_size, z_size))
    {
        //bigint_comba_mul9(z, x, y);
    }
    else if (sized_for_comba_mul<16>(x_sw, x_size, y_sw, y_size, z_size))
    {
        //bigint_comba_mul16(z, x, y);
    }
    else if (sized_for_comba_mul<24>(x_sw, x_size, y_sw, y_size, z_size))
    {
        //bigint_comba_mul24(z, x, y);
    }
    else if (x_sw < KARATSUBA_MULTIPLY_THRESHOLD ||
        y_sw < KARATSUBA_MULTIPLY_THRESHOLD ||
        !workspace)
    {
        basecase_mul(z, z_size, x, x_sw, y, y_sw);
    }
    else
    {
        const size_t N = karatsuba_size(z_size, x_size, x_sw, y_size, y_sw);

        if (N && z_size >= 2 * N && ws_size >= 2 * N)
            karatsuba_mul(z, x, y, N, workspace);
        else
            basecase_mul(z, z_size, x, x_sw, y, y_sw);
    }*/
    // else {
    //     printf("Other situation.");
    // }
}

void raw_mul(word a[4], word b[4], word r[8]) {
    bigint_mul(r, 8,
        a, 4, 4,
        b, 4, 4);
}
