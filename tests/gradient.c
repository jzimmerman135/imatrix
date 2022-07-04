#include <stdlib.h>
#include <stdio.h>
#include "imatrix.h"
#include "vismatrix.h"
/* tests have no arguments */
int main()
{
    struct iMatrix_t imat = iMatrix_new(16, 16);

    for (int i = 0; i < iMatrix_size(imat); i++)
        imat.data[i] = i;

    Vismatrix_t implot = Vismatrix_add(NULL, &imat);

    Vismatrix_dump(implot);

    Vismatrix_free(&implot);
    iMatrix_free(imat);

    return 0;
}
