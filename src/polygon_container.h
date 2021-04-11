#ifndef POLYGON_CONTAINER_H
#define POLYGON_CONTAINER_H
#include "polygon.h"
#include <SDL2/SDL.h>

struct polygon_container {
  struct polygon* polygons;
  unsigned nPolygons;
  size_t mem_polygons;
};

struct polygon_container* polygon_container_new(unsigned nPolygons, unsigned nVertices);
void polygon_container_delete(struct polygon_container* pc);
void polygon_container_reset(struct polygon_container* pc, unsigned nPolygons, unsigned nVertices);
void polygon_container_resize(struct polygon_container* pc, unsigned nPolygons);
void polygon_container_append(struct polygon_container* pc, struct polygon* o);
void polygon_container_pop(struct polygon_container* pc);
void polygon_container_render(struct polygon_container* pc, SDL_Renderer* renderer);

#endif
