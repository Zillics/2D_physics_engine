#include "polygon.h"
#include <stdio.h>
#include <stdlib.h>
#include "winding_number_algorithm.h"

#define PI 3.141592654

struct polygon* new_square(int x, int y, unsigned width) {
  double points[4][2] = { {x, y}, {x + width, y}, { x + width, y - width }, { x, y - width } };
  struct color clr = { .r = 255, .g = 255, .b = 255 };
  struct polygon* ret = new_polygon(4, points, clr);
  return ret;
}

struct polygon* new_polygon(unsigned nPoints, double points[nPoints][2], struct color clr) {
  struct polygon* o = malloc(sizeof(struct polygon));
  o->points = *matrix_new(3, nPoints, 0.0); // 1.0 for making points homogenous
  for(unsigned i = 0; i < nPoints; i++) {
    *matrix_element(&o->points, 0, i) = points[i][0];
    *matrix_element(&o->points, 1, i) = points[i][1];
    *matrix_element(&o->points, 2, i) = 1.0;
  }
  o->color = clr;
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

void polygon_centroid(struct polygon* o, double* x, double* y) {
  *x = 0.0;
  *y = 0.0;
  unsigned N = polygon_nPoints(o);
  for(unsigned int i = 0; i < N; i++) {
    *x += polygon_point(o, i)[0];
    *y += polygon_point(o, i)[1];
  }
  *x /= (double)N;
  *y /= (double)N;
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
  double xMid, yMid;
  polygon_centroid(o, &xMid, &yMid);
  struct matrix T1 = translation_matrix_2D(-xMid, -yMid);
  struct matrix R = rotation_matrix_2D(rad);
  struct matrix T2 = translation_matrix_2D(xMid, yMid);
  struct matrix transformations[4] = { T1, R, T2, o->points };
  o->points = matrices_multiply(4, transformations);
}

void polygon_translate(struct polygon* o, double x, double y) {
  double xMid, yMid;
  polygon_centroid(o, &xMid, &yMid);
  struct matrix T = translation_matrix_2D(x, y);
  o->points = matrix_multiply(&T, &o->points);
}

void polygon_render(struct polygon* o, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
  unsigned N = polygon_nPoints(o);
  for(unsigned i1 = 0; i1 < N; i1++) {
    unsigned i2 = (i1 + 1) % N;
    SDL_RenderDrawLine(renderer, polygon_point(o, i1)[0], polygon_point(o, i1)[1], polygon_point(o, i2)[0], polygon_point(o, i2)[1]);
  }
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

