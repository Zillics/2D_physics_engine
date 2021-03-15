#include "object_container.h"


struct object_container* new_object_container() {
  struct object_container* oc = malloc(sizeof(struct object_container));
  oc->objects = NULL;
  oc->nObjects = 0;
  oc->mem_objects = 0;
  return oc;
}

void delete_object_container(struct object_container* oc) {
  for(unsigned i = 0; i < oc->nObjects; i++) {
    delete_object(oc->objects + i);
  }
  free(oc);
}

void insert_square(struct object_container* oc, unsigned x, unsigned y, unsigned width) {
  struct object* o = new_square(x, y, width);
  size_t size = object_size(o);
  oc->objects = realloc(oc->objects, oc->mem_objects + size);
  oc->objects[oc->nObjects] = *o;
  oc->mem_objects += size;
  oc->nObjects += 1;
}

void pop_object(struct object_container* oc, unsigned i) {

}


void render_objects(struct object_container* oc, SDL_Renderer* renderer) {
  for(unsigned int i = 0; i < oc->nObjects; i++) {
    render_object(oc->objects + i, renderer);
  }
}
