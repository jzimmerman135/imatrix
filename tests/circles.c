#include <stdlib.h>
#include <stdio.h>
#include "imatrix.h"
#include "vismatrix.h"
/* tests have no arguments */
int main()
{
    struct iMatrix_t imat = iMatrix_new(500, 500);
    Vismatrix_t implot = Vismatrix_add(NULL, &imat);

    for (int i = 0; i < iMatrix_size(imat); i++) {
        int x = i % imat.x;
        int y = i / imat.y;
        imat.data[i] = (x * x + y * y) % 256;
    }

    Vismatrix_dump(implot);

    Vismatrix_free(&implot);
    iMatrix_free(imat);

    return 0;
}
