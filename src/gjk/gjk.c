#include "gjk.h"

#include <math.h>
#include <float.h>

static const gjk_vec2 GJK_ORIGIN = { 0.0f, 0.0f };

static gjk_vec2 gjk_furthest_point_circle(const gjk_shape* shape, gjk_vec2 d)
{
    gjk_vec2 n = gjk_normalize(d);
    return (gjk_vec2) { shape->center.x + shape->radius * n.x, shape->center.y + shape->radius * n.y };
}

static gjk_vec2 gjk_furthest_point_poly(const gjk_shape* shape, gjk_vec2 d)
{
    float max_dot = gjk_dot_product(d, shape->vertices[0]);
    size_t index = 0;
    for (size_t i = 1; i < shape->count; ++i)
    {
        float dot = gjk_dot_product(d, shape->vertices[i]);
        if (dot > max_dot)
        {
            max_dot = dot;
            index = i;
        }
    }
    return shape->vertices[index];
}

gjk_vec2 gjk_furthest_point(const gjk_shape* shape, gjk_vec2 d)
{
    switch (shape->type)
    {
    case GJK_CIRCLE: return gjk_furthest_point_circle(shape, d);
    case GJK_POLY:   return gjk_furthest_point_poly(shape, d);
    default:         return (gjk_vec2) { 0.0f, 0.0f };
    }
}

gjk_vec2 gjk_minkowski_difference(const gjk_shape* s1, const gjk_shape* s2, gjk_vec2 d)
{
    gjk_vec2 a = gjk_furthest_point(s1, d);
    gjk_vec2 b = gjk_furthest_point(s2, gjk_negate(d));

    return gjk_sub(a, b);
}

void gjk_circle(gjk_shape* shape, gjk_vec2 center, float radius)
{
    shape->type = GJK_CIRCLE;
    shape->center = center;
    shape->radius = radius;
}

void gjk_poly(gjk_shape* shape, gjk_vec2* vertices, size_t count)
{
    shape->type = GJK_POLY;
    shape->vertices = vertices;
    shape->count = count;
    shape->center = gjk_get_centroid(vertices, count);
}

void gjk_set_center(gjk_shape* shape, gjk_vec2 center)
{
    gjk_vec2 d = gjk_sub(center, shape->center);
    shape->center = center;

    if (shape->type == GJK_CIRCLE) return;

    for (size_t i = 0; i < shape->count; ++i)
        shape->vertices[i] = gjk_add(shape->vertices[i], d);
}

uint8_t gjk_collision(const gjk_shape* s1, const gjk_shape* s2, gjk_vec2* simplex_ptr)
{
    gjk_vec2 d = gjk_sub(s2->center, s1->center);

    gjk_vec2 simplex[3] = { gjk_minkowski_difference(s1, s2, d), 0.0f, 0.0f };
    size_t simplex_size = 1;

    d = gjk_sub(GJK_ORIGIN, simplex[0]);

    gjk_vec2 A, AB, AC, AO, ABperp, ACperp;
    while (1)
    {
        A = simplex[simplex_size++] = gjk_minkowski_difference(s1, s2, d);
        if (gjk_dot_product(A, d) < 0) return 0;

        if (simplex_size == 2)
        {
            AB = gjk_sub(simplex[0], A);
            AO = gjk_sub(GJK_ORIGIN, A);
            d = gjk_triple_product(AB, AO, AB);
        }
        else
        {
            AB = gjk_sub(simplex[1], A);
            AC = gjk_sub(simplex[0], A);
            AO = gjk_sub(GJK_ORIGIN, A);
            ABperp = gjk_triple_product(AC, AB, AB);
            ACperp = gjk_triple_product(AB, AC, AC);

            if (gjk_dot_product(ABperp, AO) > 0) /* region AB */
            {
                /* remove simplex[0] */
                simplex[0] = simplex[1];
                simplex[1] = simplex[2];
                simplex_size--;
                d = ABperp;
            }
            else if (gjk_dot_product(ACperp, AO) > 0) /* region AC */
            {
                /* remove simplex[1] */
                simplex[1] = simplex[2];
                simplex_size--;
                d = ACperp;
            }
            else
            {
                if (simplex_ptr)
                {
                    simplex_ptr[0] = simplex[0];
                    simplex_ptr[1] = simplex[1];
                    simplex_ptr[2] = simplex[2];
                }
                return 1;
            }
        }
    }

    return 0;
}

float epa_closest_edge(gjk_vec2* polytope, size_t size, epa_edge* edge)
{
    float dmin = FLT_MAX;
    for (size_t i = 0; i < size; ++i)
    {
        size_t j = (i + 1) % size;

        gjk_vec2 e = gjk_sub(polytope[j], polytope[i]);
        gjk_vec2 n = gjk_normalize(gjk_triple_product(e, polytope[i], e));
        float dist = gjk_dot_product(n, polytope[i]);
        if (dist < dmin)
        {
            dmin = dist;
            if (edge)
            {
                edge->p = polytope[i];
                edge->q = polytope[j];
                edge->n = n;
                edge->index = j;
            }
        }
    }

    return dmin;
}

static void epa_polytope_insert(gjk_vec2* polytope, size_t size, size_t index, gjk_vec2 p)
{
    for (size_t i = size - 1; i > index; i--)
        polytope[i] = polytope[i - 1];

    polytope[index] = p;
}

#define TOLERANCE 0.00001f

float epa(const gjk_shape* s1, const gjk_shape* s2, const gjk_vec2* simplex, gjk_vec2* normal)
{
    gjk_vec2 polytope[32] = { simplex[0], simplex[1], simplex[2] };
    size_t size = 3;
    // loop to find the collision information
    while (1)
    {
        if (size >= 32) return -1.0f;
        // obtain the feature (edge for 2D) closest to the 
        // origin on the Minkowski Difference
        epa_edge e;
        float edge_dist = epa_closest_edge(polytope, size, &e);
        // obtain a new support point in the direction of the edge normal

        gjk_vec2 p = gjk_minkowski_difference(s1, s2, e.n);
        // check the distance from the origin to the edge against the
        // distance p is along e.normal
        float dist = gjk_dot_product(p, e.n);

        if (dist - edge_dist < TOLERANCE)
        {
            // the tolerance should be something positive close to zero (ex. 0.00001)

            // if the difference is less than the tolerance then we can
            // assume that we cannot expand the simplex any further and
            // we have our solution
            *normal = e.n;
            return dist;
        }
        else
        {
            epa_polytope_insert(polytope, ++size, e.index, p);
            // we haven't reached the edge of the Minkowski Difference
            // so continue expanding by adding the new point to the simplex
            // in between the points that made the closest edge
        }
    }
    return 0.0f;
}

gjk_vec2 gjk_add(gjk_vec2 a, gjk_vec2 b)      { a.x += b.x; a.y += b.y; return a; }
gjk_vec2 gjk_sub(gjk_vec2 a, gjk_vec2 b)      { a.x -= b.x; a.y -= b.y; return a; }
gjk_vec2 gjk_negate(gjk_vec2 v)               { v.x = -v.x; v.y = -v.y; return v; }
gjk_vec2 gjk_normalize(gjk_vec2 a)            { float n = sqrtf(a.x * a.x + a.y * a.y); return (gjk_vec2) { a.x / n, a.y / n }; }
gjk_vec2 gjk_perpendicular(gjk_vec2 v)        { return (gjk_vec2) { v.y, -v.x }; }
float gjk_dot_product(gjk_vec2 a, gjk_vec2 b) { return a.x * b.x + a.y * b.y; }
float gjk_length_Squared(gjk_vec2 v)          { return v.x * v.x + v.y * v.y; }

gjk_vec2 gjk_triple_product(gjk_vec2 a, gjk_vec2 b, gjk_vec2 c)
{
    float ac = a.x * c.x + a.y * c.y; // perform a dot c
    float bc = b.x * c.x + b.y * c.y; // perform b dot c

    // perform b * (a dot c) - a * (b dot c)
    gjk_vec2 result;
    result.x = b.x * ac - a.x * bc;
    result.y = b.y * ac - a.y * bc;
    return result;
}

gjk_vec2 gjk_get_centroid(gjk_vec2* vertices, size_t count)
{
    float det = 0;
    gjk_vec2 centroid = { 0.0f };

    for (size_t i = 0; i < count; ++i)
    {
        // closed polygon
        size_t j = (i + 1) % count;

        // compute the determinant
        float temp_det = vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
        det += temp_det;

        centroid.x += (vertices[i].x + vertices[j].x) * temp_det;
        centroid.y += (vertices[i].y + vertices[j].y) * temp_det;
    }

    // divide by the total mass of the polygon
    centroid.x /= 3 * det;
    centroid.y /= 3 * det;

    return centroid;
}
