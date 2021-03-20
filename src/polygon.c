#include "polygon.h"
#include <stdio.h>
#include "winding_number_algorithm.h"

#define PI 3.141592654

struct polygon* new_square(unsigned x, unsigned y, unsigned width) {
  struct polygon* o = malloc(sizeof(struct polygon));
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

void polygon_delete(struct polygon* o) {
  matrix_delete(&o->points);
}


unsigned polygon_nPoints(struct polygon* o) {
  return o->points.cols;
}
double* polygon_point(struct polygon* o, unsigned i) {
  return o->points.data + i * o->points.rows;
}

bool polygon_contains(struct polygon* o, double* p) {
  return wn_PnPoly(p, o->points, o->points.cols) != 0;
}

void polygon_rotate(struct polygon* o, double deg) {
  polygon_rotate_deg(o, deg);
}

void polygon_rotate_deg(struct polygon* o, double deg) {
  double rad = (deg / 360) * 2 * PI;
  polygon_rotate_rad(o, rad);
}

void polygon_rotate_rad(struct polygon* o, double rad) {
  struct matrix R = rotation_matrix(rad);
  o->points = matrix_multiply(&R, &o->points); 
}

void polygon_render(struct polygon* o, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
  unsigned N = polygon_nPoints(o);
  for(unsigned i = 0; i < N - 1; i++) {
    SDL_RenderDrawLine(renderer, polygon_point(o, i)[0], polygon_point(o, i)[1], polygon_point(o, i + 1)[0], polygon_point(o, i + 1)[1]);
  }
  SDL_RenderDrawLine(renderer, polygon_point(o, N-1)[0], polygon_point(o, N-1)[1], polygon_point(o, 0)[0], polygon_point(o, 0)[1]);
}


size_t polygon_size(struct polygon* o) {
  return sizeof(struct polygon) + matrix_size(&o->points);
}

void polygon_print(struct polygon* o) {
  printf("points:\n");
  for(unsigned i = 0; i < polygon_nPoints(o); i++) {
    printf("\t(%f, %f)\n", polygon_point(o, i)[0], polygon_point(o, i)[1]);
  }
}


