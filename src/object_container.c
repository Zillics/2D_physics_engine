#include "object_container.h"

struct object_container* object_container_new() {
  struct object_container* oc = malloc(sizeof(struct object_container));
  oc->objects = NULL;
  oc->nObjects = 0;
  oc->mem_objects = 0;
  return oc;
}

void object_container_delete(struct object_container* oc) {
  for(unsigned i = 0; i < oc->nObjects; i++) {
    object_delete(oc->objects + i);
  }
  free(oc);
}

void insert_square(struct object_container* oc, unsigned x, unsigned y, unsigned width, struct color color, double mass) {
  struct object* o = new_square_object(x, y, width, color, mass);
  size_t size = object_size(o);
  oc->objects = realloc(oc->objects, oc->mem_objects + size);
  oc->objects[oc->nObjects] = *o;
  oc->mem_objects += size;
  oc->nObjects += 1;
}

void pop_polygon(struct object_container* oc, unsigned i) {

}

void render_objects(struct object_container* oc, SDL_Renderer* renderer) {
  for(unsigned int i = 0; i < oc->nObjects; i++) {
    object_render(oc->objects + i, renderer);
  }
}
