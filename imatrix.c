#include <stdlib.h>
#include "imatrix.h"
#include "assert.h"
#include <string.h>

/******************************************************************************
                                int2_t
******************************************************************************/

struct int2_t int2_(int x, int y)
{
    struct int2_t i = {x, y};
    return i;
}

int int2_is_equal(int2 a, int2 b)
{
    return a.x == b.x && a.y == b.y;
}

/******************************************************************************
                               imatrix_t
******************************************************************************/

#define INT_MIN 0x80000000
#define INT_MAX 0x7fffffff

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) > (b)) ? (a) : (b))

/*-----------------------------------------------------------------------------
                            Alloc and Free
-----------------------------------------------------------------------------*/

imatrix_t imatrix_new(struct int2_t shape)
{
    /* alloc matrix and 2 extra ints */
    int *imat = malloc(sizeof(int) * (shape.x * shape.y + 2));
    assert(imat);

    /* hide matrix size */
    imat[0] = shape.x;
    imat[1] = shape.y;
    imat += 2;

    memset(imat, 0, sizeof(int) * shape.x * shape.y);
    return imat;
}

void imatrix_free(imatrix_t *imat_pp)
{
    assert(imat_pp && *imat_pp);

    int *imat = *imat_pp;
    *imat_pp = NULL;

    /* free including hidden size info */
    imat -= 2;
    free(imat);
}

imatrix_t imatrix_resize(imatrix_t imat, struct int2_t shape)
{
    imatrix_t result = imatrix_new(shape);
    imatrix_paste(result, imat, 0);
    imatrix_free(&imat);
    return result;
}

/*-----------------------------------------------------------------------------
                                 Getters
-----------------------------------------------------------------------------*/

int imatrix_w(imatrix_t imat)
{
    return *(imat - 2);
}

int imatrix_h(imatrix_t imat)
{
    return *(imat - 1);
}

int imatrix_size(imatrix_t imat)
{
    imat -= 2;
    return imat[0] * imat[1];
}

int2 imatrix_shape(imatrix_t imat)
{
    int2 shape;
    shape.x = imatrix_w(imat);
    shape.y = imatrix_h(imat);
    return shape;
}

int imatrix_same_shape(imatrix_t a, imatrix_t b)
{
    if (!a || !b)
        return 0;

    int a_x = *(a - 2);
    int a_y = *(a - 1);

    int b_x = *(b - 2);
    int b_y = *(b - 1);

    return (a_x == b_x) && (a_y == b_y);
}

int *imatrix_at(imatrix_t imat, int x, int y)
{
    assert(imat);

    int w = imatrix_w(imat);
    return &imat[x + y * w];
}

int imatrix_binary_dot(imatrix_t val_mat, imatrix_t bin_mat)
{
    assert(val_mat && bin_mat);
    assert(imatrix_same_shape(val_mat, bin_mat));

    int sum = 0;

    int size = imatrix_size(val_mat);
    for (int i = 0; i < size; i++)
        if (bin_mat[i])
            sum += val_mat[i];

    return sum;
}

int imatrix_max(imatrix_t imat)
{
    int max = INT_MIN;
    int size = imatrix_size(imat);
    for (int i = 0; i < size; i++)
        if (imat[i] > max)
            max = imat[i];

    return max;
}

int imatrix_min(imatrix_t imat)
{
    int min = INT_MAX;
    int size = imatrix_size(imat);
    for (int i = 0; i < size; i++)
        if (imat[i] < min)
            min = imat[i];

    return min;
}

int imatrix_maxval_index(imatrix_t imat)
{
    int max = INT_MIN;
    int max_idx = 0;

    int size = imatrix_size(imat);
    for (int i = 0; i < size; i++)
        if (imat[i] > max) {
            max = imat[i];
            max_idx = i;
        }

    return max_idx;
}

/*-----------------------------------------------------------------------------
                            imatrix Modifiers
-----------------------------------------------------------------------------*/

void imatrix_or(imatrix_t dest, imatrix_t src)
{
    assert(dest && src);
    assert(imatrix_same_shape(dest, src));

    int size = imatrix_size(dest);
    for (int i = 0; i < size; i++)
        dest[i] |= src[i];
}

void imatrix_inv_or(imatrix_t dest, imatrix_t src)
{
    assert(dest && src);
    assert(imatrix_same_shape(dest, src));

    int size = imatrix_size(dest);
    for (int i = 0; i < size; i++)
        dest[i] |= ~src[i];
}

void imatrix_uniform(imatrix_t imat, int base, int variance)
{
    assert(imat);
    int size = imatrix_size(imat);
    for (int i = 0; i < size; i++)
        imat[i] = base - variance + (rand() % (2 * variance));
}

void imatrix_zero(imatrix_t imat)
{
    assert(imat);
    int size = imatrix_size(imat);
    memset(imat, 0, sizeof(int) * size);
}

void imatrix_to_bool(imatrix_t imat)
{
    assert(imat);
    int size = imatrix_size(imat);
    for (int i = 0; i < size; i++)
        imat[i] = (imat[i] != 0);
}

void imatrix_paste(imatrix_t dest, imatrix_t src, int start_point)
{
    assert(dest && src);

    int2 dest_shape = imatrix_shape(dest);
    int2 src_shape = imatrix_shape(src);

    int2 paste_shape = { min(src_shape.x, start_point % dest_shape.x),
                         min(src_shape.y, start_point / dest_shape.y) };

    for (int i = 0; i < paste_shape.y; i++) {
        int *dest_start = dest + start_point + (dest_shape.x * i);
        int *src_start  = src + (src_shape.x * i);
        memcpy(dest_start, src_start, paste_shape.x * sizeof(int));
    }
}

/******************************************************************************
                             Visualizations
******************************************************************************/

/*-----------------------------------------------------------------------------
                    Command Line Based visualizations
-----------------------------------------------------------------------------*/
/* Single Dimensional print */
void imatrix_printarr(FILE *fp, imatrix_t imat)
{
    assert(imat);
    int size = imatrix_size(imat);

    fprintf(fp, "[");
    for (int i = 0; i < size; i++)
        fprintf(fp, "%i, ", imat[i]);
    fprintf(fp, "\b\b]\n");
}

/* DARE YOU TO TRY FIX THIS */
void imatrix_print(FILE *fp, imatrix_t imat)
{
    imat-=2;int w=imat[0];int h=imat[1];imat+=2;fprintf(fp, "[ ");for(int j=0;j
    <w;j++)fprintf(fp,"%i, ",imat[j]);fprintf(fp,"\n");for(int i=1;i<h-1;i++){
    fprintf(fp,"  ");for(int j=0;j<w;j++)fprintf(fp,"%i, ",imat[j+i*w]);fprintf
    (fp,"\n");}fprintf(fp,"  ");for(int j=0;j<w;j++)fprintf(fp,"%i, ",imat[j+w*
    (h-1)]);fprintf(fp,"\b\b ]\n");
}

/*-----------------------------------------------------------------------------
                          PPM Based visualizations
-----------------------------------------------------------------------------*/
#define MIN_COLOR 0x04397a
#define MAX_COLOR 0xabf092
#define R_MASK 0x00ff0000
#define G_MASK 0x0000ff00
#define B_MASK 0x000000ff

/* keeps int b between a and c, inclusively */
static inline int boundi(int a, int b, int c)
{
    if (c < b) return c;
    if (b < a) return a;
    return b;
}

void imatrix_visualize(FILE *fp, imatrix_t imat, int min, int max)
{
    assert(fp);

    int w = imatrix_w(imat);
    int h = imatrix_h(imat);
    int n_pixels = w * h;
    int *pixels = malloc(sizeof(int) * n_pixels * 3);

    /* min and max intensities for each color value */
    int r_min = (MIN_COLOR & R_MASK) >> 16;
    int g_min = (MIN_COLOR & G_MASK) >> 8;
    int b_min = (MIN_COLOR & B_MASK) >> 0;

    int r_max = (MAX_COLOR & R_MASK) >> 16;
    int g_max = (MAX_COLOR & G_MASK) >> 8;
    int b_max = (MAX_COLOR & B_MASK) >> 0;

    for (int i = 0; i < n_pixels; i++) {
        int value = boundi(min, imat[i], max);
        float intensity = (float)(value - min) / (float)(max - min);

        /* linear interpolate between min and max colors */
        pixels[3 * i + 0] = r_min * (1 - intensity) + r_max * intensity;
        pixels[3 * i + 1] = g_min * (1 - intensity) + g_max * intensity;
        pixels[3 * i + 2] = b_min * (1 - intensity) + b_max * intensity;
    }

    fprintf(fp, "P6\n# by Jacob Zimmerman's vismatrix\n%i %i\n 255\n", w, h);
    /* write bytes */
    for (int i = 0; i < n_pixels * 3; i++) {
        fputc(pixels[i], fp);
    }

    free(pixels);
}

/* TODO: easy optimize minmax */
void imatrix_ppmdump(imatrix_t imat)
{
    imatrix_visualize(stdout, imat, imatrix_min(imat), imatrix_max(imat));
}

/*-----------------------------------------------------------------------------
                              imatrix plot
             overlay multiple imatrixes into a single image
-----------------------------------------------------------------------------*/
struct imatrixplot_t {
    imatrix_t pixels;
    int n_matrixes;
    int2 *subshapes;
    int  *start_points;
};

#define MARGIN 10

imatrixplot_t imatrixplot_add(imatrixplot_t implot, imatrix_t imat)
{
    if (implot == NULL) {
        implot = malloc(sizeof(struct imatrixplot_t));
        implot->n_matrixes = 0;
        int2 init_shape = {2 * MARGIN, 2 * MARGIN};
        implot->pixels = imatrix_new(init_shape);
    }

    /* Add to the plot */
    int n_elem = ++implot->n_matrixes;
    implot->subshapes = realloc(implot->subshapes, sizeof(int2) * n_elem);
    implot->start_points = realloc(implot->start_points, sizeof(int) * n_elem);

    int2 subplot = imatrix_shape(imat);
    implot->subshapes[n_elem - 1] = subplot;

    /* Resizing the plot */
    int2 plot = imatrix_shape(implot->pixels);
    plot.x += subplot.x + MARGIN;
    plot.y = max(plot.y, subplot.y + 2 * MARGIN);

    implot->pixels = imatrix_resize(implot->pixels, plot);

    /* Set start point to the end of the image */
    implot->start_points[n_elem - 1] = plot.x + MARGIN * plot.x;

    return implot;
}

void imatrixplot_free(imatrixplot_t *implot_pp)
{
    imatrixplot_t implot = *implot_pp;
    *implot_pp = NULL;

    imatrix_free(&implot->pixels);
    free(implot->subshapes);
    free(implot->start_points);
    free(implot);
}

void imatrixplot_put(imatrixplot_t implot, imatrix_t imat, int index)
{
    assert(implot && imat);
    assert(int2_is_equal(implot->subshapes[index], imatrix_shape(imat)));

    imatrix_paste(implot->pixels, imat, implot->start_points[index]);
}

void imatrixplot_ppmdump(imatrixplot_t implot)
{
    imatrix_ppmdump(implot->pixels);
}
