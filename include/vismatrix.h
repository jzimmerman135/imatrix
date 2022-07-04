#ifndef VISMATRIX_H
#define VISMATRIX_H

#include "imatrix.h"

#define VISMATRIX_DARK  0x22252a
#define VISMATRIX_LIGHT 0x00ffffff

#define T Vismatrix_t
typedef struct T *T;

extern T Vismatrix_add(T plot, iMatrix_t imat);
extern void Vismatrix_free(T *plot_pp);
extern void Vismatrix_set_scale(T plot, int scale);
extern void Vismatrix_set_minmax(T plot, int min, int max);
extern void Vismatrix_set_background(T plot, uint32_t color);
extern void Vismatrix_dump(T plot);

#undef T

#endif
