#ifndef ARENA_H
#define ARENA_H
#include <SDL2/SDL.h>
#include "polygon.h"
#include "object_container.h"
#include "collision_response.h"

struct arena {
  struct object_container objects;
  struct polygon border;
  double t;
};

struct arena* arena_new(double x_min, double y_min, double x_max, double y_max);
void arena_delete(struct arena* a);
void arena_tick(struct arena* a, double dt);
void arena_render(struct arena* a, SDL_Renderer* renderer);

#endif
