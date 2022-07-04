#ifndef INT2_H
#define INT2_H

typedef struct int2_t int2;
struct int2_t {
    int x;
    int y;
};

struct int2_t int2_(int x, int y);
extern int int2_is_equal(int2 a, int2 b);

#endif
