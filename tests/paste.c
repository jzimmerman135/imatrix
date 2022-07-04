#include <stdlib.h>
#include <stdio.h>
#include "imatrix.h"

/* tests have no arguments */
int main()
{
    srand(13);
    struct iMatrix_t dest = iMatrix_new(10, 12);

    struct iMatrix_t src = iMatrix_new(5, 5);
    iMatrix_uniform(src, 10, 5);

    iMatrix_print(stdout, dest);
    iMatrix_print(stdout, src);

    iMatrix_paste(dest, src, int2_(9, 5));

    iMatrix_print(stdout, dest);

    return 0;
}
