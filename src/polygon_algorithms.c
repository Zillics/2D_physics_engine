#include "polygon_algorithms.h"
#include "winding_number_algorithm.h"
#include "utils.h"

bool polygon_axes_overlap(struct polygon* o1, struct polygon* o2) {
    for(unsigned axis = 0; axis < polygon_nVertices(o1); axis++) {
      // 1. Compute normal of current vertix
      struct matrix normal = polygon_edge_normal(o1, axis);
      // 2. Compute projections of o1 and o2 vertices on normal
      struct matrix vertices1 = matrix_transpose(&o1->vertices);
      struct matrix vertices2 = matrix_transpose(&o2->vertices);
      struct matrix projections1 = matrix_multiply(&vertices1, &normal);
      struct matrix projections2 = matrix_multiply(&vertices2, &normal);
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
  return true;
}

bool separated_axis_theorem(struct polygon* o1, struct polygon* o2) {
  bool overlap1 = polygon_axes_overlap(o1, o2);
  bool overlap2 = polygon_axes_overlap(o2, o1);
  return overlap1 && overlap2;
}


void ear_clipping(struct polygon* o, struct polygon* ears, unsigned* nEars) {
  struct polygon *copy = polygon_copy(o);
  int N = polygon_nVertices(copy);
  *nEars = 0;
  int remainingVertices = N;
  for(int i = 0; (i < remainingVertices) && (remainingVertices > 3); i++) {
    if(is_an_ear(copy, i)) {
      unsigned i1 = modulo(i-1, remainingVertices);
      unsigned i2 = i;
      unsigned i3 = (i + 1) % remainingVertices;
      unsigned vertex_idx[3] = { i1, i2, i3 };
      struct polygon* triangle = polygon_create_sub(copy, 3, vertex_idx);
      ears[*nEars] = *triangle;
      *nEars += 1;
      polygon_remove_vertex(copy, i);
      i = 0;
      remainingVertices -= 1;
    }
  }
  polygon_delete(copy);
}

bool is_an_ear(struct polygon* o, int i2) {
  unsigned N = polygon_nVertices(o);
  int i1 = modulo(i2-1, N);
  int i3 = (i2+1) % N;
  // 1. Is angle between edge i-1 and i convex?
  double angle = polygon_edge_angle(o, i1, i2);
  if(angle > M_PI) {
    return false;
  }
  double points[3][2] =   { {polygon_vertex(o, i1)[0], polygon_vertex(o, i1)[1]},
                            {polygon_vertex(o, i2)[0], polygon_vertex(o, i2)[1]},
                            {polygon_vertex(o, i3)[0], polygon_vertex(o, i3)[1]}
                          };
  struct matrix* triangle = matrix_create(2, 3, points);
  // 2. Does triangle contain any of the polygon vertices?
  for(unsigned i = 0; i < N - 3; i++) {
    unsigned idx = (i3 + i + 1) % N;
    if(wn_PnPoly(polygon_vertex(o, idx), triangle)) {
      return false;
    }
  }
  return true;
}


bool GJK(struct polygon* o1, struct polygon* o2) {
  // 0. Initialize
  struct matrix* v12 = matrix_new(2, 1, 0.0);
  struct matrix* v13 = matrix_new(2, 1, 0.0);
  struct matrix* v1o = matrix_new(2, 1, 0.0);
  struct matrix* n12 = matrix_new(2, 1, 0.0);
  struct matrix* n13 = matrix_new(2, 1, 0.0);
  // 1. Choose random direction and find support point p1 in that direction
  struct matrix* simplex[3] = { NULL, NULL, NULL };
  struct matrix d1 = matrix_subtract(&o2->centroid, &o1->centroid);
  struct matrix p1 = support_point(o1, o2, &d1);
  simplex[0] = &p1;
  // 2. From p1, get direction towards origin
  struct matrix d2 = matrix_multiply_scalar(&p1, -1);
  while(true) {
    // 3. Get support point of that direction
    struct matrix p2 = support_point(o1, o2, &d2);
    simplex[1] = &p2;
    // CHECK: is p2 on other side of origin from p1?
    if(!passes_origin(&p1, &p2)) {
      return false;
    }
    // 4.
    if(simplex[2] == NULL) {
      // Line case
      *v12 = matrix_subtract(simplex[1], simplex[0]);
      *v1o = matrix_multiply_scalar(simplex[0], -1.0);
      d2 = triple_cross_product(v12, v1o, v12);
    } else {
      // Triangle case
      *v12 = matrix_subtract(simplex[1], simplex[0]);
      *v13 = matrix_subtract(simplex[2], simplex[0]);
      *v1o = matrix_multiply_scalar(simplex[0], -1.0);
      *n12 = triple_cross_product(v13, v12, v12);
      *n13 = triple_cross_product(v12, v13, v13);
      if(vector_dot(v12, v1o) > 0) {
        matrix_delete(simplex[2]);
        simplex[2] = NULL;
        d2 = *n12;
      } else if(vector_dot(v13, v1o) > 0) {
        matrix_delete(simplex[2]);
        simplex[2] = NULL;
        d2 = *n13;
      } else {
        return true;
      }
    }
  }
}


struct matrix furthest_point(struct polygon* o, struct matrix* dir) {
  double largest_dot = vector_dot_raw(matrix_col_raw(&o->vertices, 0), dir->data, 2);
  int idx = 0;
  unsigned N = polygon_nVertices(o);
  for(unsigned i = 1; i < N; i++) {
    double tmp = vector_dot_raw(matrix_col_raw(&o->vertices, i), dir->data, 2);
    if(tmp > largest_dot) {
      largest_dot = tmp;
      idx = i;
    }
  }
  struct matrix* p = vector_create(matrix_col_raw(&o->vertices, idx), 2);
  return *p;
}

struct matrix support_point(struct polygon* o1, struct polygon* o2, struct matrix* direction) {
  struct matrix p1 = furthest_point(o1, direction);
  struct matrix direction2 = matrix_multiply_scalar(direction, -1.0);
  struct matrix p2 = furthest_point(o2, &direction2);
  return matrix_subtract(&p1, &p2);
}

bool passes_origin(struct matrix* p1, struct matrix* p2) {
  struct matrix d = matrix_multiply_scalar(p2, -1.0);
  return vector_dot(p1, &d) > 0; 
}

struct matrix triple_cross_product(struct matrix* A, struct matrix* B, struct matrix* C) {
  struct matrix AxB = matrix_cross_product(A, B);
  return matrix_cross_product(&AxB, C);
}

