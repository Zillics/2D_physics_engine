#ifndef OBJECT_CONTAINER_H
#define OBJECT_CONTAINER_H
#include "object.h"
#include <SDL2/SDL.h>

struct object_container {
  struct object* objects;
  unsigned nObjects;
  size_t mem_objects;
};

struct object_container* object_container_new();
void object_container_delete(struct object_container* oc);
void object_container_insert(struct object_container* oc, struct object* o);
void object_container_pop(struct object_container* oc);
void object_container_render(struct object_container* oc, SDL_Renderer* renderer);

#endif
