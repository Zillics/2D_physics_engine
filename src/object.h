#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "color.h"
#include "linear_algebra.h"

struct object {
  struct matrix points;
  struct color color;
};

struct object* new_square(unsigned x, unsigned y, unsigned width);
size_t object_size(struct object* o);
void object_delete(struct object* o);
void object_render(struct object* o, SDL_Renderer* renderer);
unsigned object_nPoints(struct object* o);
double* object_point(struct object* o, unsigned i);
bool object_contains(struct object* o, double* p);
void object_rotate(struct object* o, double deg);
void object_rotate_rad(struct object* o, double rad);
void object_rotate_deg(struct object* o, double deg);

void object_print(struct object* o);


#endif
