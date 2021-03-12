#include "algorithm.h"

int32_t extended_euclid(int32_t a, int32_t b, int32_t* s, int32_t* t)
{
    int32_t r0 = a, r1 = b;
    int32_t s0 = 1, s1 = 0;
    int32_t t0 = 0, t1 = 1;

    int32_t q, i;

    while (r1 > 0)
    {
        q = r0 / r1;

        i = r0;
        r0 = r1;
        r1 = i - (q * r1);

        i = s0;
        s0 = s1;
        s1 = i - (q * s1);

        i = t0;
        t0 = t1;
        t1 = i - (q * t1);
    }

    *s = s0;
    *t = t0;

    return r0;
}