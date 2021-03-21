#ifndef OBJECT_H
#define OBJECT_H
#include <SDL2/SDL.h>
#include "polygon.h"
#include "linear_algebra.h"
#include "color.h"

struct object {
  struct polygon shape;
  double mass;
  struct matrix vel;
  struct matrix acc;
  struct matrix angular_vel;
  struct matrix angular_acc;
};

void object_init(struct object* o);
void object_delete(struct object* o);
size_t object_size(struct object* o);
void object_render(struct object* o, SDL_Renderer* renderer);
struct object* new_square_object(double x, double y, double width, struct color color, double mass);

#endif

