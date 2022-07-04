#include <stdlib.h>
#include <stdio.h>
#include "imatrix.h"
#include "vismatrix.h"

/* tests have no arguments */
int main()
{
    struct iMatrix_t imat1 = iMatrix_new(10, 10);
    struct iMatrix_t imat2 = iMatrix_new(20, 20);
    struct iMatrix_t imat3 = iMatrix_new(10, 15);

    Vismatrix_t plot = Vismatrix_add(NULL, &imat1);
    plot = Vismatrix_add(plot, &imat2);
    plot = Vismatrix_add(plot, &imat3);

    Vismatrix_set_scale(plot, 10);
    Vismatrix_set_minmax(plot, 0, 100);
    Vismatrix_set_background(plot, VISMATRIX_LIGHT);

    for (int i = 0; i < 100; i++) {
        iMatrix_uniform(imat1, 25, 25);
        iMatrix_uniform(imat2, 75, 25);
        iMatrix_uniform(imat3, 50, 50);

        Vismatrix_dump(plot);
    }

    Vismatrix_free(&plot);

    return 0;
}
