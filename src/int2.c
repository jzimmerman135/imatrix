#include "int2.h"

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
