#include "polygon_algorithms.h"
#include "winding_number_algorithm.h"
#include "utils.h"

#define EPA_MAX_VERTICES 100

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
  double points[3][2] =   { {polygon_vertex_raw(o, i1)[0], polygon_vertex_raw(o, i1)[1]},
                            {polygon_vertex_raw(o, i2)[0], polygon_vertex_raw(o, i2)[1]},
                            {polygon_vertex_raw(o, i3)[0], polygon_vertex_raw(o, i3)[1]}
                          };
  struct matrix* triangle = matrix_create(2, 3, points);
  // 2. Does triangle contain any of the polygon vertices?
  for(unsigned i = 0; i < N - 3; i++) {
    unsigned idx = (i3 + i + 1) % N;
    if(wn_PnPoly(polygon_vertex_raw(o, idx), triangle)) {
      return false;
    }
  }
  return true;
}

bool GJK(struct polygon* o1, struct polygon* o2, double simplex_ret[3][2]) {
  // 0. Initialize
  struct matrix* v12 = vector_new(3, 0.0);
  struct matrix* v13 = vector_new(3, 0.0);
  struct matrix* v1o = vector_new(3, 0.0);
  struct matrix* n12 = vector_new(3, 0.0);
  struct matrix* n13 = vector_new(3, 0.0);
  struct matrix* n32 = vector_new(3, 0.0);
  struct matrix* n31 = vector_new(3, 0.0);
  struct matrix* v32 = vector_new(3, 0.0);
  struct matrix* v31 = vector_new(3, 0.0);
  struct matrix* v3o = vector_new(3, 0.0);
  struct matrix* p1 = vector_new(3, 0.0);
  struct matrix* p2 = vector_new(3, 0.0);
  struct matrix* p3 = vector_new(3, 0.0);
  struct matrix* simplex[3] = { p1, p2, p3 };
  unsigned n = 0;
  unsigned i = 0;
  unsigned max_iterations = 100;
  // 1. Choose random direction and find support point p1 in that direction
  struct matrix d1 = matrix_subtract(&o2->centroid, &o1->centroid);
  d1 = vector_normalize(&d1);
  *simplex[0] = support_point(o1, o2, &d1);
  *simplex[0] = vector_normalize(simplex[0]);
  n += 1;
  // 2. From p1, get direction towards origin
  struct matrix d2 = matrix_multiply_scalar(simplex[0], -1);
  d2 = vector_normalize(&d2);
  while(i < max_iterations) {
    // 3. Get support point of that direction
    *simplex[n] = support_point(o1, o2, &d2);
    *simplex[n] = vector_normalize(simplex[n]);
    // CHECK: is p2 on other side of origin from p1?
    if(vector_dot(simplex[n], &d2) < 0) {
      return false;
    }
    n += 1;
    // 4.
    if(n == 2) {
      // Line case
      *v12 = matrix_subtract(simplex[1], simplex[0]);
      *v12 = vector_normalize(v12);
      *v1o = matrix_multiply_scalar(simplex[0], -1.0);
      *v1o = vector_normalize(v1o);
      d2 = triple_cross_product(v12, v1o, v12);
      d2 = vector_normalize(&d2);
    } else {
      // Triangle case
      *v32 = matrix_subtract(simplex[1], simplex[2]);
      *v32 = vector_normalize(v32);
      *v31 = matrix_subtract(simplex[0], simplex[2]);
      *v31 = vector_normalize(v31);
      *v3o = matrix_multiply_scalar(simplex[2], -1.0);
      *v3o = vector_normalize(v3o);
      *n32 = triple_cross_product(v31, v32, v32);
      *n32 = vector_normalize(n32);
      *n31 = triple_cross_product(v32, v31, v31);
      *n31 = vector_normalize(n31);
      if(vector_dot(n32, v3o) > 0) {
        n -= 1;
        d2 = *v32;
      } else if(vector_dot(n31, v3o) > 0) {
        n -= 1;
        d2 = *n31;
      } else {
        if(simplex_ret) {
          simplex_ret[0][0] = simplex[0]->data[0];
          simplex_ret[0][1] = simplex[0]->data[1];
          simplex_ret[1][0] = simplex[1]->data[0];
          simplex_ret[1][1] = simplex[1]->data[1];
          simplex_ret[2][0] = simplex[2]->data[0];
          simplex_ret[2][1] = simplex[2]->data[1];
        }
        return true;
      }
    }
    i += 1;
  }
  printf("GJK EXCEEDED MAX ITERATIONS!!!!!!!!\n");
  return false;
}

struct matrix EPA(struct polygon* poly1, struct polygon* poly2, double simplex[3][2]) {
  double tol = 1e-6;
  unsigned iter = 0;
  unsigned max_iterations = 100;
  unsigned nVertices = 3;
  struct polygon* poly = polygon_new(3, simplex);
  struct matrix* penetration_vector = vector_new(3, 0);
  while(iter < max_iterations && nVertices < EPA_MAX_VERTICES) {
    int iClosest = closest_edge(nVertices, poly);
    struct matrix e = polygon_edge(poly, iClosest);
    struct matrix n = polygon_edge_normal(poly, iClosest);
    struct matrix p = support_point(poly1, poly2, &n);
    double d = vector_dot(&p, &n);
    if((d - vector_norm(&e)) < tol) {
      *penetration_vector = matrix_multiply_scalar(&n, d);
      return *penetration_vector;
    } else {
      polygon_insert_vertex(poly, p.data, iClosest);
    }
    iter += 1;
  }
  printf("EPA EXCEEDED MAX ITERATIONS!!!!!!!!\n");
  return *penetration_vector;
}


int closest_edge(unsigned N, struct polygon* poly) {
  double closest = 1e10;
  int iClosest = -1;
  for(unsigned i = 0; i < N; i++) {
    struct matrix e = polygon_edge(poly, i);
    struct matrix p1 = polygon_vertex(poly, i);
    struct matrix n = triple_cross_product(&e, &p1, &e);
    n = vector_normalize(&n);
    double d = vector_dot(&n, &p1);
    if(d < closest) {
      closest = d;
      iClosest = i;
    }
  }
  return iClosest;
}

struct matrix furthest_point(struct polygon* o, struct matrix* dir) {
  double largest_dot = vector_dot_raw(matrix_col_raw(&o->vertices, 0), dir->data, 3);
  int idx = 0;
  unsigned N = polygon_nVertices(o);
  for(unsigned i = 1; i < N; i++) {
    double tmp = vector_dot_raw(matrix_col_raw(&o->vertices, i), dir->data, 3);
    if(tmp > largest_dot) {
      largest_dot = tmp;
      idx = i;
    }
  }
  struct matrix* p = vector_create(matrix_col_raw(&o->vertices, idx), 3);
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

