#include <stdlib.h>
#include "imatrix.h"
#include "assert.h"
#include "brickutils.h"
#include <string.h>

#define T iMatrix_t

/******************************************************************************
                               iMatrix_t
******************************************************************************/

#define INT_MIN 0x80000000
#define INT_MAX 0x7fffffff

// #define max(a, b) (((a) > (b)) ? (a) : (b))
// #define min(a, b) (((a) > (b)) ? (a) : (b))

/*-----------------------------------------------------------------------------
                            Alloc and Free
-----------------------------------------------------------------------------*/

struct T iMatrix_new(int w, int h)
{
    int *data = malloc(sizeof(int) * w * h);
    assert(data);

    struct T imat = {w, h, data};
    memset(data, 0, sizeof(int) * w * h);
    return imat;
}

void iMatrix_free(struct T imat)
{
    if (imat.data != NULL)
        free(imat.data);
}

struct T iMatrix_resize(struct T imat, int w, int h)
{
    struct T result = iMatrix_new(w, h);
    iMatrix_paste(result, imat, int2_(0, 0));
    iMatrix_free(imat);
    return result;
}

struct T iMatrix_copy(struct T src)
{
    int *data = malloc(sizeof(int) * iMatrix_size(src));
    assert(data && src.data);

    struct T dest = {src.x, src.y, data};
    memcpy(data, src.data, sizeof(int) * iMatrix_size(src));

    return dest;
}

/*-----------------------------------------------------------------------------
                                 Getters
-----------------------------------------------------------------------------*/

int iMatrix_w(struct T imat)
{
    return imat.x;
}

int iMatrix_h(struct T imat)
{
    return imat.y;
}

int iMatrix_size(struct T imat)
{
    return imat.x * imat.y;
}

int2 iMatrix_shape(struct T imat)
{
    int2 shape = {imat.x, imat.y};
    return shape;
}

int iMatrix_same_shape(struct T a, struct T b)
{
    return (a.x == b.x) && (a.y == b.y);
}

int *iMatrix_at(struct T imat, int x, int y)
{
    assert(imat.data);
    int w = iMatrix_w(imat);
    return &imat.data[x + y * w];
}

int iMatrix_binary_dot(struct T val_mat, struct T bin_mat)
{
    assert(iMatrix_same_shape(val_mat, bin_mat));

    int sum = 0;

    int size = iMatrix_size(val_mat);
    int *bin_data = bin_mat.data;
    int *val_data = val_mat.data;

    for (int i = 0; i < size; i++)
        if (bin_data[i])
            sum += val_data[i];

    return sum;
}

int iMatrix_max(struct T imat)
{

    int max = INT_MIN;

    int size = iMatrix_size(imat);
    int *data = imat.data;

    for (int i = 0; i < size; i++)
        if (data[i] > max)
            max = data[i];

    return max;
}

int iMatrix_min(struct T imat)
{
    int min = INT_MAX;
    int size = iMatrix_size(imat);
    int *data = imat.data;
    for (int i = 0; i < size; i++)
        if (data[i] < min)
            min = data[i];

    return min;
}

int iMatrix_max_index(struct T imat)
{
    int max = INT_MIN;
    int max_idx = 0;

    int size = iMatrix_size(imat);
    int *data = imat.data;

    for (int i = 0; i < size; i++)
        if (data[i] > max) {
            max = data[i];
            max_idx = i;
        }

    return max_idx;
}

int iMatrix_min_index(struct T imat)
{
    int min = INT_MAX;
    int min_idx = 0;

    int size = iMatrix_size(imat);
    int *data = imat.data;

    for (int i = 0; i < size; i++)
        if (data[i] < min) {
            min = data[i];
            min_idx = i;
        }

    return min_idx;
}


/*-----------------------------------------------------------------------------
                            iMatrix Modifiers
-----------------------------------------------------------------------------*/

void iMatrix_set_all(struct T imat, int value)
{
    int size = iMatrix_size(imat);
    int *data = imat.data;

    for (int i = 0; i < size; i++)
        data[i] = value;
}

void iMatrix_map(struct T imat,
                 void apply(int index, struct T imat, int elem, void *cl),
                 void *cl)
{
    int size = iMatrix_size(imat);
    int *data = imat.data;

    for (int i = 0; i < size; i++)
        apply(i, imat, data[i], cl);
}

void iMatrix_or(struct T dest, struct T src)
{
    assert(iMatrix_same_shape(dest, src));

    int size = iMatrix_size(dest);
    int *dest_data = dest.data;
    int *src_data = src.data;

    for (int i = 0; i < size; i++)
        dest_data[i] |= src_data[i];
}

void iMatrix_inv_or(struct T dest, struct T src)
{
    assert(iMatrix_same_shape(dest, src));

    int size = iMatrix_size(dest);
    int *dest_data = dest.data;
    int *src_data = src.data;

    for (int i = 0; i < size; i++)
        dest_data[i] |= ~src_data[i];
}

void iMatrix_uniform(struct T imat, int base, int variance)
{
    int size = iMatrix_size(imat);
    int *data = imat.data;

    for (int i = 0; i < size; i++)
        data[i] = base - variance + (rand() % (2 * variance));
}

void iMatrix_zero(struct T imat)
{
    int size = iMatrix_size(imat);
    memset(imat.data, 0, sizeof(int) * size);
}

void iMatrix_to_bool(struct T imat)
{
    assert(imat.data);
    int size = iMatrix_size(imat);
    int *data = imat.data;

    for (int i = 0; i < size; i++)
        data[i] = (data[i] != 0);
}

void iMatrix_paste(struct T dest, struct T src, int2 start)
{
    verify(start.x + src.x <= dest.x, "iMatrix_paste x dimension overflow");
    verify(start.y + src.y <= dest.y, "iMatrix_paste y dimension overflow");

    int *dest_data = dest.data;
    int *src_data = src.data;

    for (int i = 0; i < src.y; i++) {
        int *dest_line = start.x + (dest.x * (start.y + i)) + dest_data;
        int *src_line = (src.x * i) + src_data;

        for (int j = 0; j < src.x; j++) {
            dest_line[j] = src_line[j];
        }
    }
}

/******************************************************************************
                             Visualizations
******************************************************************************/

/*-----------------------------------------------------------------------------
                    Command Line Based visualizations
-----------------------------------------------------------------------------*/
/* Single Dimensional print */
void iMatrix_printarr(FILE *fp, struct T imat)
{
    int size = iMatrix_size(imat);
    int *data = imat.data;

    fprintf(fp, "[");
    for (int i = 0; i < size; i++)
        fprintf(fp, "%i, ", data[i]);
    fprintf(fp, "\b\b]\n");
}

/* DARE YOU TO TRY FIX THIS */
void iMatrix_print(FILE *fp, struct T x)
{
    int *imat=x.data;int w=x.x;int h=x.y;fprintf(fp, "[ ");for(int j=0;j
    <w;j++)fprintf(fp,"%i, ",imat[j]);fprintf(fp,"\n");for(int i=1;i<h-1;i++){
    fprintf(fp,"  ");for(int j=0;j<w;j++)fprintf(fp,"%i, ",imat[j+i*w]);fprintf
    (fp,"\n");}fprintf(fp,"  ");for(int j=0;j<w;j++)fprintf(fp,"%i, ",imat[j+w*
    (h-1)]);fprintf(fp,"\b\b ]\n");
}

#undef T
