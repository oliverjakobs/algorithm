#ifndef GJK_H
#define GJK_H

#include <stdint.h>

typedef struct
{
    float x, y;
} gjk_vec2;

gjk_vec2 gjk_add(gjk_vec2 a, gjk_vec2 b);
gjk_vec2 gjk_sub(gjk_vec2 a, gjk_vec2 b);
gjk_vec2 gjk_negate(gjk_vec2 v);
gjk_vec2 gjk_normalize(gjk_vec2 a);
gjk_vec2 gjk_perpendicular(gjk_vec2 v);
float gjk_dot_product(gjk_vec2 a, gjk_vec2 b);
float gjk_length_Squared(gjk_vec2 v);

gjk_vec2 gjk_triple_product(gjk_vec2 a, gjk_vec2 b, gjk_vec2 c);
gjk_vec2 gjk_get_centroid(gjk_vec2* vertices, size_t count);

typedef enum
{
    GJK_CIRCLE,
    GJK_POLY
} gjk_shape_type;

typedef struct
{
    gjk_shape_type type;
    gjk_vec2 center;
    union
    {
        float radius;
        struct
        {
            gjk_vec2* vertices;
            size_t count;
        };
    };
} gjk_shape;

void gjk_circle(gjk_shape* shape, gjk_vec2 center, float radius);
void gjk_poly(gjk_shape* shape, gjk_vec2* vertices, size_t count);

void gjk_set_center(gjk_shape* shape, gjk_vec2 center);

gjk_vec2 gjk_furthest_point(const gjk_shape* shape, gjk_vec2 d);
gjk_vec2 gjk_minkowski_difference(const gjk_shape* s1, const gjk_shape* s2, gjk_vec2 d);
uint8_t gjk_collision(const gjk_shape* s1, const gjk_shape* s2, gjk_vec2* simplex_ptr);

typedef struct
{
    gjk_vec2 p;
    gjk_vec2 q;
    gjk_vec2 n;
    size_t index;
} epa_edge;

float epa_closest_edge(gjk_vec2* polytope, size_t size, epa_edge* e);
float epa(const gjk_shape* s1, const gjk_shape* s2, const gjk_vec2* simplex, gjk_vec2* n);

#endif // !GJK_H
