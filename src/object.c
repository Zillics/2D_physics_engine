#include "object.h"
#include <stdio.h>

struct object* new_square(unsigned x, unsigned y, unsigned width) {
  struct object* o = malloc(sizeof(struct object));
  o->points = malloc(4 * 2 * sizeof(unsigned));
  o->points[0 + 0 * 2] = x;
  o->points[1 + 0 * 2] = y;
  o->points[0 + 1 * 2] = x+width;
  o->points[1 + 1 * 2] = y;
  o->points[0 + 2 * 2] = x+width;
  o->points[1 + 2 * 2] = y-width;
  o->points[0 + 3 * 2] = x;
  o->points[1 + 3 * 2] = y-width;
  o->nPoints = 4;
  o->color.r = 255;
  o->color.g = 255;
  o->color.b = 255;
  return o;
}

void delete_object(struct object* o) {
  free(o->points);
}

void render_object(struct object* o, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
  for(unsigned i = 0; i < o->nPoints - 1; i++) {
    SDL_RenderDrawLine(renderer, o->points[0 + i * 2], o->points[1 + i * 2], o->points[0 + (i + 1) * 2], o->points[1 + (i + 1) * 2]);
  }
  SDL_RenderDrawLine(renderer, o->points[0 + (o->nPoints-1) * 2], o->points[1 + (o->nPoints-1) * 2], o->points[0 + 0 * 2], o->points[1 + 0 * 2]);
}


size_t object_size(struct object* o) {
  return sizeof(struct object) + o->nPoints * 2 * sizeof(unsigned);
}

void print_object(struct object* o) {
  printf("points:\n");
  for(unsigned i = 0; i < o->nPoints; i++) {
    printf("\t(%d, %d)\n", o->points[0 + i * 2], o->points[1 + i * 2]);
  }
}


