#ifndef POLYGON_H
#define POLYGON_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "color.h"
#include "linear_algebra.h"

struct polygon {
  struct matrix points;
  struct color color;
};

struct polygon* new_square(int x, int y, unsigned width, struct color color);
struct polygon* new_polygon(unsigned nPoints, double points[2][nPoints], struct color color);
size_t polygon_size(struct polygon* o);
void polygon_delete(struct polygon* o);
void polygon_render(struct polygon* o, SDL_Renderer* renderer);
unsigned polygon_nPoints(struct polygon* o);
double* polygon_point(struct polygon* o, unsigned i);
/** Find normal vector of vertix between point i and point i + 1. */
struct matrix polygon_vertix_normal(struct polygon* o, unsigned i);
void polygon_centroid(struct polygon* o, double* x, double* y);
bool polygon_contains(struct polygon* o, double* p);
void polygon_rotate(struct polygon* o, double deg);
void polygon_translate(struct polygon* o, double* v, double k);
void polygon_rotate_rad(struct polygon* o, double rad);
void polygon_rotate_deg(struct polygon* o, double deg);
void polygon_print(struct polygon* o);

bool polygons_collide_(struct polygon* o1, struct polygon* o2);

bool polygons_collide(unsigned N, struct polygon polygons[N]);

#endif
