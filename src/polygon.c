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

struct matrix polygon_edge_normal(struct polygon* o, unsigned i, bool inward) {
  int dir;
  if(inward) {
    dir = -1;
  } else {
    dir = 1;
  }
  struct matrix* n = matrix_new(3, 1, 0.0);
  // Swap x and y axes to get normal
  unsigned i2 = (i + 1) % polygon_nPoints(o);
  *matrix_element(n, 0, 0) = polygon_point(o, i2)[0] - polygon_point(o, i)[0];
  *matrix_element(n, 1, 0) = polygon_point(o, i2)[1] - polygon_point(o, i)[1];
  matrix_rotate(n, dir * 90);
  struct matrix ret = unit_vector(n);
  *matrix_element(&ret, 2, 0) = 1.0;
  return ret;
}

struct matrix polygon_edge_midpoint(struct polygon* o, unsigned i) {
  struct matrix* m = matrix_new(3, 1, 0.0);
  unsigned i2 = (i + 1) % polygon_nPoints(o);
  double* p1 = polygon_point(o, i);
  double* p2 = polygon_point(o, i2);
  *matrix_element(m, 0, 0) = 0.5 * (p1[0] + p2[0]);
  *matrix_element(m, 1, 0) = 0.5 * (p1[1] + p2[1]);
  *matrix_element(m, 2, 0) = 1.0;
  return *m;
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

void polygon_translate(struct polygon* o, double* v, double k) {
  double xMid, yMid;
  polygon_centroid(o, &xMid, &yMid);
  struct matrix T = translation_matrix_2D(k*v[0], k*v[1]);
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

bool polygons_collide(unsigned N, struct polygon polygons[N]) {
  for(unsigned i = 0; i < N; i++) {
    struct polygon* o1 = polygons + i;
    struct polygon* o2 = polygons + (i + 1) % N;
    for(unsigned axis = 0; axis < polygon_nPoints(o1); axis++) {
      // 1. Compute normal of current vertix
      struct matrix normal = polygon_edge_normal(o1, axis, true);
      // 2. Compute projections of o1 and o2 points on normal
      struct matrix points1 = matrix_transpose(&o1->points);
      struct matrix points2 = matrix_transpose(&o2->points);
      struct matrix projections1 = matrix_multiply(&points1, &normal);
      struct matrix projections2 = matrix_multiply(&points2, &normal);
      // 3. Check if there is overlap betwen projections
      double min1 = matrix_min(&projections1);
      double max1 = matrix_max(&projections1);
      double min2 = matrix_min(&projections2);
      double max2 = matrix_max(&projections2);
      bool overlaps = fmax(min1, min2) < fmin(max1, max2);
      if(!overlaps) {
        return false;
      }
    }
  }
  return true;
}
