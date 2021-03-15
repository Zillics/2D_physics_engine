#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"
#include <SDL2/SDL.h>

struct object {
  struct point* points;
  unsigned nPoints;
};

struct object* new_square(unsigned x, unsigned y, unsigned width);
size_t object_size(struct object* o);
void delete_object(struct object* o);
void render_object(struct object* o, SDL_Renderer* renderer);
void print_object(struct object* o);


#endif
