#ifndef IMATRIX_H
#define IMATRIX_H

#include <stdio.h>
#include "int2.h"

#define T iMatrix_t

typedef struct T *T;

struct T {
    int x;
    int y;
    int *data;
};

/* Alloc and Free
 * -------------
 *  note: do not try and free T using C standard free(imat);
 *        gonna segfault or something.
 */
extern struct T iMatrix_new(int w, int h);
extern void iMatrix_free(struct T imat);
extern struct T iMatrix_resize(struct T imat, int w, int h);

/* Getters
 * -------
 * at is for 2d indexing
 */
extern int iMatrix_w(struct T imat);
extern int iMatrix_h(struct T imat);
extern int *iMatrix_data(struct T imat);
extern int iMatrix_size(struct T imat);
extern int2 iMatrix_shape(struct T imat);
extern int *iMatrix_at(struct T imat, int x, int y);

/* Values
 * ------
 *
 */
extern int iMatrix_same_shape(struct T a, struct T b);
extern int iMatrix_binary_dot(struct T val_mat, struct T bin_mat);
extern int iMatrix_max_index(struct T imat);
extern int iMatrix_min_index(struct T imat);

/* Modifiers
 * ---------
 * These ones will modify the first argument, in place
 */
extern void iMatrix_set_all(struct T imat, int value);
extern void iMatrix_or(struct T dest, struct T src);
extern void iMatrix_inv_or(struct T dest, struct T src);
extern void iMatrix_uniform(struct T imat, int base, int variance);
extern void iMatrix_zero(struct T imat);
extern void iMatrix_to_bool(struct T imat);
extern void iMatrix_paste(struct T dest, struct T src, int2 start);
extern void iMatrix_map(struct T imat,
                     void apply(int index, struct T imat, int value, void *cl),
                     void *cl);

/* Visualizations
 * --------------
 * print corresponds to terminal output
 * printarr is 1d representation
 * visualize lets you control ppm output
 * ppmdump drops ppm to stdout
 */
extern void iMatrix_print(FILE *fp, struct T imat);
extern void iMatrix_printarr(FILE *fp, struct T imat);

#undef T
#endif
