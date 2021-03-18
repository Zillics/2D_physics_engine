#include "object.h"
#include <stdio.h>
#include "winding_number_algorithm.h"

#define PI 3.141592654

struct object* new_square(unsigned x, unsigned y, unsigned width) {
  struct object* o = malloc(sizeof(struct object));
  o->points = *matrix_new(2, 4, 0.0);
  *matrix_element(&o->points, 0, 0) = x;
  *matrix_element(&o->points, 1, 0) = y;
  *matrix_element(&o->points, 0, 1) = x + width;
  *matrix_element(&o->points, 1, 1) = y;
  *matrix_element(&o->points, 0, 2) = x + width;
  *matrix_element(&o->points, 1, 2) = y - width;
  *matrix_element(&o->points, 0, 3) = x;
  *matrix_element(&o->points, 1, 3) = y - width;
  o->color.r = 255;
  o->color.g = 255;
  o->color.b = 255;
  return o;
}

void object_delete(struct object* o) {
  matrix_delete(&o->points);
}


unsigned object_nPoints(struct object* o) {
  return o->points.cols;
}
double* object_point(struct object* o, unsigned i) {
  return o->points.data + i * o->points.rows;
}

bool object_contains(struct object* o, double* p) {
  return wn_PnPoly(p, o->points, o->points.cols) != 0;
}

void object_rotate(struct object* o, double deg) {
  object_rotate_deg(o, deg);
}

void object_rotate_deg(struct object* o, double deg) {
  double rad = (deg / 360) * 2 * PI;
  object_rotate_rad(o, rad);
}

void object_rotate_rad(struct object* o, double rad) {
  struct matrix R = rotation_matrix(rad);
  o->points = matrix_multiply(&R, &o->points); 
}

void object_render(struct object* o, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
  unsigned N = object_nPoints(o);
  for(unsigned i = 0; i < N - 1; i++) {
    SDL_RenderDrawLine(renderer, object_point(o, i)[0], object_point(o, i)[1], object_point(o, i + 1)[0], object_point(o, i + 1)[1]);
  }
  SDL_RenderDrawLine(renderer, object_point(o, N-1)[0], object_point(o, N-1)[1], object_point(o, 0)[0], object_point(o, 0)[1]);
}


size_t object_size(struct object* o) {
  return sizeof(struct object) + matrix_size(&o->points);
}

void object_print(struct object* o) {
  printf("points:\n");
  for(unsigned i = 0; i < object_nPoints(o); i++) {
    printf("\t(%f, %f)\n", object_point(o, i)[0], object_point(o, i)[1]);
  }
}


