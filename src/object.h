#ifndef OBJECT_H
#define OBJECT_H
#include <SDL2/SDL.h>
#include "polygon.h"
#include "linear_algebra.h"
#include "color.h"

struct object {
  struct polygon shape;
  double mass;
  bool collides;
  /** x, y, dx, dy, ddx, ddy, a, da, dda */
  struct matrix state;
  struct matrix direction;
};

struct object* object_generate(unsigned nPoints, double r);
struct object* new_square_object(double width, struct color color, double mass);
void object_delete(struct object* o);
void object_reset_state(struct object* o);
size_t object_size(struct object* o);
void object_render(struct object* o, SDL_Renderer* renderer);
void object_tick(struct object* o, double dt);
bool objects_collide(struct object* o1, struct object* o2);
void object_place(struct object* o, double x, double y);
void object_translate(struct object* o, double* v, double k);
void object_rotate(struct object* o, double deg);
#endif

