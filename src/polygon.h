#ifndef POLYGON_H
#define POLYGON_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "color.h"
#include "linear_algebra.h"

struct polygon {
  struct matrix vertices;
  struct matrix edge_midpoints;
  struct matrix edge_normals;
  bool edge_normals_inward;
  struct color color;
};

struct polygon* new_square(int x, int y, unsigned width, struct color color);
struct polygon* polygon_new(unsigned nVertices, double vertices[2][nVertices], struct color color);
size_t polygon_size(struct polygon* o);
void polygon_delete(struct polygon* o);
void polygon_render(struct polygon* o, SDL_Renderer* renderer);
unsigned polygon_nVertices(struct polygon* o);
double* polygon_point(struct polygon* o, unsigned i);
/** Find normal vector of vertix between point i and point i + 1. 
 * Set inward to true/false to specify whether normal points inward/outward. */
struct matrix polygon_edge_normal(struct polygon* o, unsigned i);
struct matrix polygon_edge_midpoint(struct polygon* o, unsigned i);
struct matrix polygon_compute_edge_normal(struct polygon* o, unsigned i, bool inward);
struct matrix polygon_compute_edge_midpoint(struct polygon* o, unsigned i);
void polygon_recompute_edge_normals(struct polygon* o, bool inward);
void polygon_centroid(struct polygon* o, double* x, double* y);
bool polygon_contains(struct polygon* o, double* p);
void polygon_rotate(struct polygon* o, double deg);
void polygon_translate(struct polygon* o, double* v, double k);
void polygon_rotate_rad(struct polygon* o, double rad);
void polygon_rotate_deg(struct polygon* o, double deg);
/** Given polygon, populate n and c with normal and center of all vertices.
 * Set inward to true/false for normals pointing inwards/outwards. */
void polygon_vertex_normals(struct polygon* o, bool inward, struct matrix* n, struct matrix* c);
void polygon_print(struct polygon* o);
bool polygons_collide(unsigned N, struct polygon polygons[N]);

#endif
