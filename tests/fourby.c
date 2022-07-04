#include <stdlib.h>
#include <stdio.h>
#include "imatrix.h"
#include "vismatrix.h"

/* tests have no arguments */
int main()
{
    struct iMatrix_t imat = iMatrix_new(20, 20);

    iMatrix_zero(imat);

    Vismatrix_t plot = Vismatrix_add(NULL, &imat);

    Vismatrix_set_background(plot, 0x22252a);

    Vismatrix_set_minmax(plot, 0, 160);


    for (int i = 0; i < 400; i++) {
        imat.data[i] = rand() % 160;
        Vismatrix_dump(plot);
    }

    for (int i = 0; i < 400 * 20; i++) {
        imat.data[i % 400] += 5;
        if (i % 20 == 0)
            Vismatrix_dump(plot);
    }

    iMatrix_print(stderr, imat);



    Vismatrix_free(&plot);


    return 0;
}
