#ifndef OBJECT_H
#define OBJECT_H
#include <SDL2/SDL.h>
#include "polygon.h"
#include "linear_algebra.h"
#include "color.h"

struct state {
  struct matrix pos;
  struct matrix vel;
  struct matrix acc;
  struct matrix angular_vel;
  struct matrix angular_acc;
};

struct object {
  struct polygon shape;
  double mass;
  struct state state;
  bool collides;
  struct matrix direction;
};

void object_delete(struct object* o);
size_t object_size(struct object* o);
void object_render(struct object* o, SDL_Renderer* renderer);
void object_tick(struct object* o, double dt);
bool objects_collide(struct object* o1, struct object* o2);
void object_translate(struct object* o, double* v, double k);
void object_rotate(struct object* o, double deg);
struct object* new_square_object(double width, struct color color, double mass);
void object_reset_state(struct object* o, double* p, double* v, double* a, double* av, double* aa);

struct state* state_new();
void state_reset(struct state* s, double* p, double* v, double* a, double* av, double* aa);
void state_delete(struct state* s);
size_t state_size(struct state* s);

#endif

