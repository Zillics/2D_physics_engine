#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "color.h"
#include "linear_algebra.h"

struct polygon {
  struct matrix points;
  struct color color;
};

struct polygon* new_square(int x, int y, unsigned width);
struct polygon* new_polygon(unsigned nPoints, double points[2][nPoints], struct color clr);
size_t polygon_size(struct polygon* o);
void polygon_delete(struct polygon* o);
void polygon_render(struct polygon* o, SDL_Renderer* renderer);
unsigned polygon_nPoints(struct polygon* o);
double* polygon_point(struct polygon* o, unsigned i);
void polygon_centroid(struct polygon* o, double* x, double*y);
bool polygon_contains(struct polygon* o, double* p);
void polygon_rotate(struct polygon* o, double deg);
void polygon_translate(struct polygon* o, double x, double y);
void polygon_rotate_rad(struct polygon* o, double rad);
void polygon_rotate_deg(struct polygon* o, double deg);

void polygon_print(struct polygon* o);


#endif
