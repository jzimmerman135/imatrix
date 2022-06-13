#ifndef IMATRIX_H
#define IMATRIX_H

#include <stdio.h>


typedef struct int2_t int2;

struct int2_t {
    int x;
    int y;
};

struct int2_t int2_(int x, int y);
int int2_is_equal(int2 a, int2 b);


typedef int *imatrix_t;

/* Alloc and Free
 * -------------
 *  note: do not try and free imatrix_t using C standard free(imat);
 *        gonna segfault or something.
*/
extern imatrix_t imatrix_new(struct int2_t shape);
extern void imatrix_free(imatrix_t *imat_pp);
extern imatrix_t imatrix_resize(imatrix_t imat, struct int2_t shape);

/* Getters
 * -------
 * at is for 2d indexing
 */
extern int imatrix_w(imatrix_t imat);
extern int imatrix_h(imatrix_t imat);
extern int imatrix_size(imatrix_t imat);
extern int2 imatrix_shape(imatrix_t imat);
extern int imatrix_same_shape(imatrix_t a, imatrix_t b);
extern int *imatrix_at(imatrix_t imat, int x, int y);
extern int imatrix_binary_dot(imatrix_t val_mat, imatrix_t bin_mat);
extern int imatrix_maxval_index(imatrix_t imat);

/* Modifiers
 * ---------
 * These ones will modify the first argument, in place
 */
extern void imatrix_or(imatrix_t dest, imatrix_t src);
extern void imatrix_inv_or(imatrix_t dest, imatrix_t src);
extern void imatrix_uniform(imatrix_t imat, int base, int variance);
extern void imatrix_zero(imatrix_t imat);
extern void imatrix_to_bool(imatrix_t imat);
extern void imatrix_paste(imatrix_t dest, imatrix_t src, int start_point);

/* Visualizations
 * --------------
 * print corresponds to terminal output
 * printarr is 1d representation
 * visualize lets you control ppm output
 * ppmdump drops ppm to stdout
 */
extern void imatrix_print(FILE *fp, imatrix_t imat);
extern void imatrix_printarr(FILE *fp, imatrix_t imat);
extern void imatrix_visualize(FILE *fp, imatrix_t imat, int min, int max);
extern void imatrix_ppmdump(imatrix_t imat);

/* Visualize multiple imatrixes together
 * -------------------------------------
 *
 */
typedef struct imatrixplot_t *imatrixplot_t;

extern imatrixplot_t imatrixplot_add(imatrixplot_t implot, imatrix_t imat);
extern void imatrixplot_free(imatrixplot_t *implot_pp);

extern void imatrixplot_background(imatrixplot_t implot, uint32_t color);

extern void imatrixplot_put(imatrixplot_t implot, imatrix_t imat, int index);
extern void imatrixplot_ppmdump(imatrixplot_t implot);



#endif
