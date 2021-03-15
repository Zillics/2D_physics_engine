#include "object.h"
#include <stdio.h>

struct object* new_square(unsigned x, unsigned y, unsigned width) {
  struct object* o = malloc(sizeof(struct object));
  o->points = malloc(4*sizeof(struct point));
  o->points[0].x = x;
  o->points[0].y = y;
  o->points[1].x = x+width;
  o->points[1].y = y;
  o->points[2].x = x+width;
  o->points[2].y = y-width;
  o->points[3].x = x;
  o->points[3].y = y-width;
  o->nPoints = 4;
  return o;
}

void delete_object(struct object* o) {
  free(o);
}

void render_object(struct object* o, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
  for(unsigned i = 0; i < o->nPoints - 1; i++) {
    SDL_RenderDrawLine(renderer, (o->points + i)->x, (o->points + i)->y, (o->points + i + 1)->x, (o->points + i + 1)->y);
  }
  SDL_RenderDrawLine(renderer, (o->points + o->nPoints - 1)->x, (o->points + o->nPoints - 1)->y, o->points->x, o->points->y);
}


size_t object_size(struct object* o) {
  return sizeof(struct object) + o->nPoints * sizeof(struct point);
}

void print_object(struct object* o) {
  printf("points:\n");
  print_points(o->points, o->nPoints); 
}


