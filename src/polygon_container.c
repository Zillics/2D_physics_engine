#include "color.h"
#include "polygon_container.h"
#include "utils.h"

struct polygon_container* polygon_container_new(unsigned nPolygons, unsigned nVertices) {
  struct polygon_container* pc = malloc(sizeof(struct polygon_container));
  pc->polygons = NULL;
  pc->nPolygons = 0;
  pc->mem_polygons = 0;
  polygon_container_reset(pc, nPolygons, nVertices);
  return pc;
}

void polygon_container_delete(struct polygon_container* pc) {
  for(unsigned i = 0; i < pc->nPolygons; i++) {
    polygon_delete(pc->polygons + i);
  }
  free(pc);
}

void polygon_container_reset(struct polygon_container* pc, unsigned nPolygons, unsigned nVertices) {
  for(unsigned i = 0; i < pc->nPolygons; i++) {
    polygon_container_pop(pc);
  }
  for(unsigned i = 0; i < nPolygons; i++) {
    double vertices[2][nVertices];
    struct color color = color_red();
    struct polygon* o = polygon_new(nVertices, vertices, color);
    polygon_container_append(pc, o);
  }
}
void polygon_container_resize(struct polygon_container* pc, unsigned nPolygons) {
  unsigned nPop = imax(0, (int)pc->nPolygons - (int)nPolygons);
  for(unsigned i = 0; i < nPop; i++) {
    polygon_container_pop(pc);
  }
}
void polygon_container_append(struct polygon_container* pc, struct polygon* o) {
  size_t size = polygon_size(o);
  pc->polygons = realloc(pc->polygons, pc->mem_polygons + size);
  pc->polygons[pc->nPolygons] = *o;
  pc->mem_polygons += size;
  pc->nPolygons += 1;
}

void polygon_container_pop(struct polygon_container* pc) {
  struct polygon* o = pc->polygons + pc->nPolygons;
  size_t size = polygon_size(o);
  polygon_delete(o);
  pc->polygons = realloc(pc->polygons, pc->mem_polygons - size); 
  pc->mem_polygons -= size;
  pc->nPolygons -= 1;
}

void polygon_container_render(struct polygon_container* pc, SDL_Renderer* renderer) {
  for(unsigned int i = 0; i < pc->nPolygons; i++) {
    polygon_render(pc->polygons + i, renderer);
  }
}
