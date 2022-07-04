#include <stdlib.h>
#include <stdio.h>
#include "imatrix.h"
#include "vismatrix.h"
/* tests have no arguments */
int main()
{
    struct iMatrix_t imat = iMatrix_new(100, 100);
    Vismatrix_t plot = Vismatrix_add(NULL, &imat);

    Vismatrix_set_background(plot, 0x22252a);
    Vismatrix_set_scale(plot, 1);

    for (int t = 0; t < 512; t++) {
        for (int i = 0; i < iMatrix_size(imat); i++) {
            int x = i % imat.x;
            int y = i / imat.y;

            imat.data[i] = (x * x + y * y) % t;
        }

        Vismatrix_dump(plot);
    }

    Vismatrix_free(&plot);
    iMatrix_free(imat);

    return 0;
}
