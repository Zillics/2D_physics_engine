#ifndef OBJECT_CONTAINER_H
#define OBJECT_CONTAINER_H
#include "polygon.h"

struct polygon_container {
  struct polygon* polygons;
  unsigned nObjects;
  size_t mem_polygons;
};

struct polygon_container* new_polygon_container();
void delete_polygon_container(struct polygon_container* oc);
void insert_square(struct polygon_container* oc, unsigned x, unsigned y, unsigned width);
void pop_polygon(struct polygon_container* oc, unsigned i);
void render_polygons(struct polygon_container* oc, SDL_Renderer* renderer);

#endif
