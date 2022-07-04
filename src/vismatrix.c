#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brickutils.h"
#include "vismatrix.h"
#include "imatrix.h"

#define T Vismatrix_t

/*-----------------------------------------------------------------------------
                          PPM Based visualizations
-----------------------------------------------------------------------------*/
#define INT_MIN 0x80000000
#define INT_MAX 0x7fffffff
#define MIN_COLOR 0x04397a
#define MAX_COLOR 0xabf092
#define WHITE  0x00ffffff

#define R_MASK 0x00ff0000
#define G_MASK 0x0000ff00
#define B_MASK 0x000000ff

#define MARGIN 2
#define PADDING 10

struct T {
    int n_matrixes;
    int scale_factor;
    int w; /* including margin */
    int h; /* including margin */

    int maxval;
    int minval;
    uint32_t background_color;

    iMatrix_t *matrixes;
    int2 *subshapes;
    int2 *paste_locs;
    struct RGB *pixels;

    struct iMatrix_t matplot;
};

struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

static T Vismatrix_empty_new();
static inline int boundi(int a, int b, int c);
static inline struct RGB RGB_(uint32_t color);
static inline void PPM_write_vismatrix(T plot);
static inline struct RGB lerp_color(struct RGB color_min, struct RGB color_max,
                                    int val_min, int val_max, int v);

T Vismatrix_add(T plot, iMatrix_t imat)
{
    if (plot == NULL) {
        plot = Vismatrix_empty_new();
    }

    /* Add to the plot */
    int n_elem = ++plot->n_matrixes;
    plot->matrixes = realloc(plot->matrixes, sizeof(imat) * n_elem);
    plot->subshapes = realloc(plot->subshapes, sizeof(int2) * n_elem);
    plot->paste_locs = realloc(plot->paste_locs, sizeof(int2) * n_elem);

    /* calculate new height and width */
    int start_x = plot->w;
    plot->w = plot->w + iMatrix_w(*imat) + MARGIN;
    plot->h = max(plot->h, iMatrix_h(*imat) + MARGIN * 2);
    int start_y = (plot->h - iMatrix_h(*imat)) / 2;

    plot->matrixes[n_elem - 1] = imat;
    plot->subshapes[n_elem - 1] = iMatrix_shape(*imat);
    plot->paste_locs[n_elem - 1] = int2_(start_x, start_y);

    for (int i = 0; i < n_elem; i++) {
        start_y = (plot->h - plot->subshapes[i].y) / 2;
        plot->paste_locs[i].y = start_y;
    }

    /* resize pixel containers */
    iMatrix_free(plot->matplot);
    plot->matplot = iMatrix_new(plot->w, plot->h);
    iMatrix_set_all(plot->matplot, INT_MIN);

    free(plot->pixels);
    plot->pixels = malloc(sizeof(struct RGB) * plot->h * plot->w);

    return plot;
}

void Vismatrix_free(T *plot_pp)
{
    T plot = *plot_pp;
    *plot_pp = NULL;

    free(plot->subshapes);
    free(plot->matrixes);
    free(plot->pixels);
    free(plot->paste_locs);
    iMatrix_free(plot->matplot);
    free(plot);
}

void Vismatrix_set_minmax(T plot, int min, int max)
{
    plot->minval = min;
    plot->maxval = max;
}

void Vismatrix_set_scale(T plot, int scale)
{
    plot->scale_factor = scale;
}

void Vismatrix_set_background(T plot, uint32_t color)
{
    plot->background_color = color; /* TODO: finish me */
}

void Vismatrix_dump(T plot)
{
    for (int i = 0; i < plot->n_matrixes; i++)
        iMatrix_paste(plot->matplot, *plot->matrixes[i], plot->paste_locs[i]);

    int v_min = plot->minval;
    int v_max = plot->maxval;
    int n_pixels = plot->w * plot->h;

    int *data = plot->matplot.data;
    struct RGB *pixels = plot->pixels;

    int is_background = INT_MIN;

    struct RGB c_min = RGB_(MIN_COLOR);
    struct RGB c_max = RGB_(MAX_COLOR);
    struct RGB c_background = RGB_(plot->background_color);

    for (int i = 0; i < n_pixels; i++) {
        if (data[i] == is_background) {
            pixels[i] = c_background;
        } else {
            int v = boundi(v_min, data[i], v_max); /* sanity */
            pixels[i] = lerp_color(c_min, c_max, v_min, v_max, v);
        }
    }

    /* write bytes */
    PPM_write_vismatrix(plot);
}

static T Vismatrix_empty_new()
{
    T plot;
    plot = malloc(sizeof(struct T));
    plot->n_matrixes = 0;
    plot->scale_factor = 2;
    plot->w      = MARGIN;
    plot->h      = 0;
    plot->minval = 0;
    plot->maxval = 256;
    plot->background_color = WHITE;
    plot->pixels       = NULL;
    plot->subshapes    = NULL;
    plot->paste_locs = NULL;
    plot->matrixes     = NULL;
    return plot;
}

/* keeps int b between a and c, inclusively */
static inline int boundi(int a, int b, int c)
{
    if (c < b) return c;
    if (b < a) return a;
    return b;
}

static inline void PPM_write_vismatrix(T plot)
{
    int scale_factor = plot->scale_factor;
    int w = plot->w;
    int h = plot->h;
    struct RGB *pixels = plot->pixels;

    printf("P6\n");
    printf("# made by Jacob Zimmerman using vismatrix\n");
    printf("%i %i\n", w * scale_factor, h * scale_factor);
    printf("%i\n", 255);

    for (int j = 0; j < h; j++) {
    for (int n = 0; n < scale_factor; n++) {
        for (int i = 0; i < w; i++) {
        for (int m = 0; m < scale_factor; m++) {
            struct RGB curr = pixels[i + w * j];
            printf("%c%c%c", curr.r, curr.g, curr.b);
        }
        }
    }
    }
}

static inline struct RGB lerp_color(struct RGB color_min, struct RGB color_max,
                                    int val_min, int val_max, int v)
{
    int v_range = val_max - val_min;

    struct RGB c_range = {
        color_max.r - color_min.r,
        color_max.g - color_min.g,
        color_max.b - color_min.b
    };

    struct RGB result_color = {
        (v_range * color_min.r + c_range.r * (v - val_min)) / v_range,
        (v_range * color_min.g + c_range.g * (v - val_min)) / v_range,
        (v_range * color_min.b + c_range.b * (v - val_min)) / v_range
    };

    return result_color;
}

static inline struct RGB RGB_(uint32_t color)
{
    int r = ((color & R_MASK) >> 16);
    int g = ((color & G_MASK) >> 8);
    int b = ((color & B_MASK) >> 0);

    struct RGB result_color = {r, g, b};
    return result_color;
}
