#include <stdlib.h>
#include <stdio.h>

#include "imatrix.h"


/* tests have no arguments */
int main()
{
	struct iMatrix_t imat = iMatrix_new(5, 5);
    iMatrix_uniform(imat, 10, 5);
    iMatrix_printarr(stdout, imat);
    iMatrix_free(imat);
    return 0;
}
