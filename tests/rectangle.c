#include <stdlib.h>
#include <stdio.h>
#include "imatrix.h"
#include "vismatrix.h"

/* tests have no arguments */
int main()
{
    struct iMatrix_t back = iMatrix_new(12, 18);
    struct iMatrix_t imat = iMatrix_new(4, 10);
    iMatrix_uniform(imat, 50, 25);
    iMatrix_print(stderr, imat);
    iMatrix_print(stderr, back);

    iMatrix_paste(back, imat, int2_(4, 4));

    iMatrix_print(stderr, back);

    Vismatrix_t plot = Vismatrix_add(NULL, &imat);

    Vismatrix_set_minmax(plot, 25, 75);
    Vismatrix_set_scale(plot, 10);

    Vismatrix_dump(plot);

    return 0;
}
