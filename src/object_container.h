#ifndef OBJECT_CONTAINER_H
#define OBJECT_CONTAINER_H
#include "object.h"

struct object_container {
  struct object* objects;
  unsigned nObjects;
  size_t mem_objects;
};

struct object_container* new_object_container();
void delete_object_container(struct object_container* oc);
void insert_square(struct object_container* oc, unsigned x, unsigned y, unsigned width);
void pop_object(struct object_container* oc, unsigned i);
void render_objects(struct object_container* oc, SDL_Renderer* renderer);

#endif
