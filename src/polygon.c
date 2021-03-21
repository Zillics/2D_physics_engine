#include "polygon.h"
#include <stdio.h>
#include <stdlib.h>
#include "winding_number_algorithm.h"

struct polygon* new_square(int x, int y, unsigned width, struct color color) {
  double points[4][2] = { {x - 0.5 * width, y + 0.5 * width},
                          {x + 0.5 * width, y + 0.5 * width},
                          {x + 0.5 * width, y - 0.5 * width},
                          {x - 0.5 * width, y - 0.5 * width} };
  struct polygon* ret = new_polygon(4, points, color);
  double xMid, yMid;
  polygon_centroid(ret, &xMid, &yMid);
  return ret;
}

struct polygon* new_polygon(unsigned nPoints, double points[nPoints][2], struct color color) {
  struct polygon* o = malloc(sizeof(struct polygon));
  o->points = *matrix_new(3, nPoints, 0.0); // 1.0 for making points homogenous
  for(unsigned i = 0; i < nPoints; i++) {
    *matrix_element(&o->points, 0, i) = points[i][0];
    *matrix_element(&o->points, 1, i) = points[i][1];
    *matrix_element(&o->points, 2, i) = 1.0;
  }
  o->color = color;
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

struct matrix polygon_vertix_normal(struct polygon* o, unsigned i) {
  struct matrix* n = matrix_new(3, 1, 0.0);
  // Swap x and y axes to get normal
  *matrix_element(n, 0, 0) = polygon_point(o, i + 1)[1] - polygon_point(o, i)[1];
  *matrix_element(n, 1, 0) = polygon_point(o, i + 1)[0] - polygon_point(o, i)[0];
  *matrix_element(n, 2, 0) = 1.0;
  return *n;
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
  double rad = (deg / 360) * 2 * M_PI;
  polygon_rotate_rad(o, rad);
}

void polygon_rotate_rad(struct polygon* o, double rad) {
  double xMid, yMid;
  polygon_centroid(o, &xMid, &yMid);
  struct matrix transformations[3] = {translation_matrix_2D(xMid, yMid),
                                      rotation_matrix_2D(rad),
                                      translation_matrix_2D(-xMid, -yMid)};
  struct matrix TRT = matrices_multiply(3, transformations);
  o->points = matrix_multiply(&TRT, &o->points);
}

void polygon_translate(struct polygon* o, double* v) {
  double xMid, yMid;
  polygon_centroid(o, &xMid, &yMid);
  struct matrix T = translation_matrix_2D(v[0], v[1]);
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

bool polygons_collide(struct polygon* o1, struct polygon* o2) {
  bool collides = true;
  for(unsigned axis = 0; axis < polygon_nPoints(o1); axis++) {
    struct matrix normal = polygon_vertix_normal(o1, axis);
    struct matrix normal_t = matrix_transpose(&normal);
    // 1. Compute projections for o1
    struct matrix projections1 = matrix_multiply(&normal_t, &o1->points);
    struct matrix min1 = matrix_rowwise_min(&projections1);
    struct matrix max1 = matrix_rowwise_max(&projections1);
    // 2. Compute projections for o2
    struct matrix projections2 = matrix_multiply(&normal_t, &o2->points);
    struct matrix min2 = matrix_rowwise_min(&projections2);
    struct matrix max2 = matrix_rowwise_max(&projections2);
    // 3. Check if there is overlap
    bool overlaps = matrix_value(&min1, 0, 0);
  }
  return true;
}
