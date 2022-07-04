#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "imatrix.h"
#include "brickutils.h"

struct RGB {
    char a;
    char b;
    char c;
};

/* tests have no arguments */
int main()
{
    struct iMatrix_t imat = iMatrix_new(5, 5);

    for (int i = 0; i < iMatrix_size(imat); i++)
        imat.data[i] = 20;

    assert(imat.data[10] == 20);
    iMatrix_printarr(stdout, imat);
    iMatrix_free(imat);

    printf("sizeof RGB is %lu \n", sizeof(struct RGB));

    return 0;
}
