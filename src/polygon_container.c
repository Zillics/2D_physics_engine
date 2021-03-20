#include "polygon_container.h"


struct polygon_container* new_polygon_container() {
  struct polygon_container* oc = malloc(sizeof(struct polygon_container));
  oc->polygons = NULL;
  oc->nObjects = 0;
  oc->mem_polygons = 0;
  return oc;
}

void delete_polygon_container(struct polygon_container* oc) {
  for(unsigned i = 0; i < oc->nObjects; i++) {
    polygon_delete(oc->polygons + i);
  }
  free(oc);
}

void insert_square(struct polygon_container* oc, unsigned x, unsigned y, unsigned width) {
  struct polygon* o = new_square(x, y, width);
  size_t size = polygon_size(o);
  oc->polygons = realloc(oc->polygons, oc->mem_polygons + size);
  oc->polygons[oc->nObjects] = *o;
  oc->mem_polygons += size;
  oc->nObjects += 1;
}

void pop_polygon(struct polygon_container* oc, unsigned i) {

}


void render_polygons(struct polygon_container* oc, SDL_Renderer* renderer) {
  for(unsigned int i = 0; i < oc->nObjects; i++) {
    polygon_render(oc->polygons + i, renderer);
  }
}
