#include "polygon.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "winding_number_algorithm.h"
#include "polygon_algorithms.h"

struct polygon* new_square(int x, int y, unsigned width, struct color color) {
  double vertices[4][2] = { {x - 0.5 * width, y + 0.5 * width},
                          {x + 0.5 * width, y + 0.5 * width},
                          {x + 0.5 * width, y - 0.5 * width},
                          {x - 0.5 * width, y - 0.5 * width} };
  struct polygon* ret = polygon_new(4, vertices, color);
  return ret;
}

struct polygon* polygon_new(unsigned nVertices, double vertices[nVertices][2], struct color color) {
  struct polygon* o = malloc(sizeof(struct polygon));
  o->vertices = *matrix_new(3, nVertices, 0.0);
  for(unsigned i = 0; i < nVertices; i++) {
    *matrix_element(&o->vertices, 0, i) = vertices[i][0];
    *matrix_element(&o->vertices, 1, i) = vertices[i][1];
    *matrix_element(&o->vertices, 2, i) = 1.0; // 1.0 for making vertices homogenous
  }
  assert(vertices_clockwise(&o->vertices));
  bool inward = false;
  polygon_recompute_edge_normals(o, inward);
  struct matrix n;
  o->edge_midpoints = *matrix_new(3, nVertices, 0.0);
  struct matrix m;
  for(unsigned i = 0; i < nVertices; i++) {
    m = polygon_compute_edge_midpoint(o, i);
    *matrix_element(&o->edge_midpoints, 0, i) = matrix_value(&m, 0, 0);
    *matrix_element(&o->edge_midpoints, 1, i) = matrix_value(&m, 1, 0);
    *matrix_element(&o->edge_midpoints, 2, i) = 1.0;
  }
  o->color = color;
  struct matrix* c = vector_new(3, 1.0);
  polygon_centroid(o, vector_element(c, 0), vector_element(c, 1));
  o->centroid = *c;
  o->area = polygon_area(o);
  matrix_print(&o->vertices);
  return o;
}

struct polygon* polygon_create_sub(struct polygon* o, unsigned nVertices, unsigned* vertex_idx) {
  double vertices[nVertices][2];
  for(unsigned i = 0; i < nVertices; i++) {
    unsigned vi = vertex_idx[i];
    vertices[i][0] = polygon_vertex(o, vi)[0];
    vertices[i][1] = polygon_vertex(o, vi)[1];
  }
  return polygon_new(nVertices, vertices, o->color);
}

struct polygon* polygon_generate(unsigned N, double r) {
  double vertices[N][2];
  double direction[3] = {0.0, r, 1.0};
  struct matrix* dir = vector_create(direction, 3);
  double rads[N];
  // Generate random angles from range 0..360 degrees
  random_doubles(0, 2*M_PI, N, rads);
  dsort(rads, N);
  double prev_rad = 0.0;
  for(unsigned i = 0; i < N; i++) {
    // 1. Rotate line s.t its angle is rads[i]
    double rad = rads[i] - prev_rad;
    prev_rad = rads[i];
    struct matrix R = rotation_matrix_2D(-rad);
    *dir = matrix_multiply(&R, dir);
    // 2. Generate a random point somewhere on line
    double k = random_double(0.2, 1.0);
    struct matrix pi = matrix_multiply_scalar(dir, k);
    vertices[i][0] = *(pi.data + 0);
    vertices[i][1] = *(pi.data + 1);
  }
  struct color color = color_green();
  return polygon_new(N, vertices, color);
}

void polygon_delete(struct polygon* o) {
  matrix_delete(&o->vertices);
  matrix_delete(&o->edge_normals);
  matrix_delete(&o->edge_midpoints);
}

unsigned polygon_nVertices(struct polygon* o) {
  return o->vertices.cols;
}
double* polygon_vertex(struct polygon* o, unsigned i) {
  return o->vertices.data + i * o->vertices.rows;
}

void polygon_edge_raw(struct polygon* o, unsigned i, double* p1, double* p2) {
  unsigned N = polygon_nVertices(o);
  p1 = polygon_vertex(o, i);
  p2 = polygon_vertex(o, (i + 1) % N);
}

struct matrix polygon_edge_normal(struct polygon* o, unsigned i) {
  return matrix_col(&o->edge_normals, i);
}

struct matrix polygon_edge_midpoint(struct polygon* o, unsigned i) {
  return matrix_col(&o->edge_midpoints, i);
}

struct matrix polygon_compute_edge_normal(struct polygon* o, unsigned i, bool inward) {
  int dir;
  if(inward) {
    dir = -1;
  } else {
    dir = 1;
  }
  struct matrix* n = matrix_new(3, 1, 0.0);
  // Swap x and y axes to get normal
  unsigned i2 = (i + 1) % polygon_nVertices(o);
  *matrix_element(n, 0, 0) = polygon_vertex(o, i2)[0] - polygon_vertex(o, i)[0];
  *matrix_element(n, 1, 0) = polygon_vertex(o, i2)[1] - polygon_vertex(o, i)[1];
  matrix_rotate(n, dir * 90);
  struct matrix ret = unit_vector(n);
  *matrix_element(&ret, 2, 0) = 1.0;
  return ret;
}

struct matrix polygon_compute_edge_midpoint(struct polygon* o, unsigned i) {
  struct matrix* m = matrix_new(3, 1, 0.0);
  unsigned i2 = (i + 1) % polygon_nVertices(o);
  double* p1 = polygon_vertex(o, i);
  double* p2 = polygon_vertex(o, i2);
  *matrix_element(m, 0, 0) = 0.5 * (p1[0] + p2[0]);
  *matrix_element(m, 1, 0) = 0.5 * (p1[1] + p2[1]);
  *matrix_element(m, 2, 0) = 1.0;
  return *m;
}

void polygon_recompute_edge_normals(struct polygon* o, bool inward) {
  o->edge_normals_inward = inward;
  unsigned N = polygon_nVertices(o);
  o->edge_normals = *matrix_new(3, N, 0.0);
  struct matrix n;
  for(unsigned i = 0; i < N; i++) {
    n = polygon_compute_edge_normal(o, i, inward);
    *matrix_element(&o->edge_normals, 0, i) = matrix_value(&n, 0, 0);
    *matrix_element(&o->edge_normals, 1, i) = matrix_value(&n, 1, 0);
    *matrix_element(&o->edge_normals, 2, i) = 1.0;
  }
}

void polygon_centroid(struct polygon* o, double* x, double* y) {
  *x = 0.0;
  *y = 0.0;
  unsigned N = polygon_nVertices(o);
  for(unsigned int i = 0; i < N; i++) {
    *x += polygon_vertex(o, i)[0];
    *y += polygon_vertex(o, i)[1];
  }
  *x /= (double)N;
  *y /= (double)N;
}

struct matrix polygon_edge(struct polygon* o, unsigned i1) {
  unsigned N = polygon_nVertices(o);
  unsigned i2 = (i1 + 1) % N;
  return vector_subtract_raw(polygon_vertex(o, i2), polygon_vertex(o, i1), 3);
}

double polygon_edge_angle(struct polygon* o, unsigned i1, unsigned i2) {
  unsigned N = polygon_nVertices(o);
  struct matrix e1 = vector_subtract_raw(polygon_vertex(o, (i1 + 1) % N), polygon_vertex(o, i1), 2);
  struct matrix e2 = vector_subtract_raw(polygon_vertex(o, (i2 + 1) % N), polygon_vertex(o, i2), 2);
  return vector_angle(&e1, &e2);
}

bool polygon_is_convex(struct polygon* o) {
  unsigned N = polygon_nVertices(o);
  for(unsigned i1 = 0; i1 < N; i1++) {
    unsigned i2 = (i1 + 1) % N;
    double angle = polygon_edge_angle(o, i2, i1);
    if(angle > M_PI) {
      return false;
    }
  }
  return true;
}

bool polygon_self_intersects(struct polygon* o, unsigned* ia1, unsigned* ia2, unsigned* ib1, unsigned* ib2) {
  printf("INTERSECTS?\n");
  return vertices_intersect(&o->vertices, ia1, ia2, ib1, ib2);
}

bool vertices_intersect(struct matrix* verts, unsigned* ia1, unsigned* ia2, unsigned* ib1, unsigned* ib2) {
  // TODO: something faster than O(n^2)
  printf("...............\n");
  unsigned N = verts->cols;
  for(unsigned i1 = 0; i1 < N; i1++) {
    *ia1 = i1;
    *ia2 = (i1 + 1) % N;
    double* a1 = matrix_col_raw(verts, *ia1);
    double* a2 = matrix_col_raw(verts, *ia2);
    for(unsigned i2 = 0; i2 < N - 1; i2++) {
      *ib1 = (i1 + 1 + i2) % N;
      *ib2 = (i1 + 1 + i2 + 1) % N;
      double* b1 = matrix_col_raw(verts, *ib1);
      double* b2 = matrix_col_raw(verts, *ib2);
      printf("%d->%d | %d->%d, N: %d\n", *ia1, *ia2, *ib1, *ib2, N);
      if(lines_intersect_2D_raw(a1, a2, b1, b2)) {
        return true;
      }
    }
  }
  return false;

}

bool vertices_clockwise(struct matrix* verts) {
  // TODO: understand why this works
  // stole the solution from: https://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order#:~:text=If%20the%20determinant%20is%20negative,q%20and%20r%20are%20collinear.
  double sum = 0.0;
  unsigned N = verts->cols;
  for(unsigned i1 = 0; i1 < N; i1++) {
    unsigned i2 = (i1 + 1) % N;
    double *p1 = matrix_col_raw(verts, i1);
    double *p2 = matrix_col_raw(verts, i2);
    sum += (p2[0] - p1[0]) * (p2[1] + p1[1]);
  }
  if(sum < 0) {
    return false;
  } else {
    return true;
  }
}

bool polygon_contains(struct polygon* o, double* p) {
  return wn_PnPoly(p, &o->vertices, o->vertices.cols) != 0;
}

void polygon_rotate(struct polygon* o, double deg) {
  polygon_rotate_deg(o, deg);
}

void polygon_rotate_deg(struct polygon* o, double deg) {
  double rad = (deg / 360) * 2 * M_PI;
  polygon_rotate_rad(o, rad);
}

void polygon_rotate_rad(struct polygon* o, double rad) {
  struct matrix R = rotation_matrix_2D(rad);
  double xMid, yMid;
  polygon_centroid(o, &xMid, &yMid);
  struct matrix transformations[3] = {translation_matrix_2D(xMid, yMid),
                                      R,
                                      translation_matrix_2D(-xMid, -yMid)};
  struct matrix TRT = matrices_multiply(3, transformations);
  o->vertices = matrix_multiply(&TRT, &o->vertices);
  o->edge_normals = matrix_multiply(&R, &o->edge_normals);
  o->edge_midpoints = matrix_multiply(&TRT, &o->edge_midpoints);
}

void polygon_translate(struct polygon* o, double* v, double k) {
  struct matrix T = translation_matrix_2D(k*v[0], k*v[1]);
  o->vertices = matrix_multiply(&T, &o->vertices);
  o->edge_midpoints = matrix_multiply(&T, &o->edge_midpoints);
  o->centroid = matrix_multiply(&T, &o->centroid);
}

void polygon_render(struct polygon* o, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
  unsigned N = polygon_nVertices(o);
  for(unsigned i1 = 0; i1 < N; i1++) {
    unsigned i2 = (i1 + 1) % N;
    // Vertix
    polygon_render_vertex(o, renderer, i1);
    // Normal
    double x1 = matrix_value(&o->edge_midpoints, 0, i1);
    double x2 = x1 + matrix_value(&o->edge_normals, 0, i1);
    double y1 = matrix_value(&o->edge_midpoints, 1, i1);
    double y2 = y1 + matrix_value(&o->edge_normals, 1, i1);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }

}

void polygon_render_vertex(struct polygon* o, SDL_Renderer* renderer, unsigned i1) {
  unsigned N = polygon_nVertices(o);
  SDL_SetRenderDrawColor(renderer, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
  unsigned i2 = (i1 + 1) % N;
  SDL_RenderDrawLine(renderer, polygon_vertex(o, i1)[0], polygon_vertex(o, i1)[1], polygon_vertex(o, i2)[0], polygon_vertex(o, i2)[1]);
}

size_t polygon_size(struct polygon* o) {
  return sizeof(struct polygon)
       + matrix_size(&o->vertices)
       + matrix_size(&o->edge_normals)
       + matrix_size(&o->edge_midpoints);
}

void polygon_print(struct polygon* o) {
  for(unsigned i = 0; i < polygon_nVertices(o); i++) {
    printf("\t(%f, %f)\n", polygon_vertex(o, i)[0], polygon_vertex(o, i)[1]);
  }
}


bool polygons_collide(struct polygon* o1, struct polygon* o2) {
  return separated_axis_theorem(o1, o2);
}

double polygon_area(struct polygon* o) {
  double area = 0.0;
  struct matrix* p1 = matrix_new(3, 1, 0.0);
  matrix_insert_col(p1, polygon_vertex(o, 0), 0.0);
  for(unsigned i = 1; i < polygon_nVertices(o) - 1; i++) {
    struct matrix* p2 = matrix_new(3, 1, 0.0);
    matrix_insert_col(p2, polygon_vertex(o, i), 0.0);
    struct matrix* p3 = matrix_new(3, 1, 0.0);
    matrix_insert_col(p3, polygon_vertex(o, i + 1), 0.0);
    double ai = triangle_area(p1, p2, p3);
    area += ai;
  }
  return area;
}

double polygon_moment_of_inertia(struct polygon* o, double ro) {
  double I = 0.0;
  struct matrix* p1 = vector_create(polygon_vertex(o, 0), 3);
  for(unsigned i = 1; i < polygon_nVertices(o) - 1; i++) {
    // Define points of triangle
    struct matrix* p2 = vector_create(polygon_vertex(o, i), 3);
    struct matrix* p3 = vector_create(polygon_vertex(o, i+1), 3);

    // Find out p4
    struct matrix v2 = matrix_subtract(p2, p1);
    struct matrix v3 = matrix_subtract(p3, p1);
    struct matrix proj23 = vector_projection(&v3, &v2);
    struct matrix p4 = matrix_add(p1, &proj23); 

    // Find out w1, h1
    struct matrix v14 = matrix_subtract(&p4, p1);
    double w1 = vector_norm_L2_raw(v14.data, 2);
    struct matrix v43 = matrix_subtract(p3, &p4);
    double h1 = vector_norm_L2_raw(v43.data, 2);

    // Find out w2, h2
    struct matrix v42 = matrix_subtract(p2, &p4);
    double w2 = vector_norm_L2_raw(v42.data, 2);
    double h2 = h1;

    // Moment of inertia of triangle1 around p3
    double I1 = triangle_moment_of_inertia(h1, w1, ro);
    // Icm = I - m*d^2
    // Moment of inertia around triangle1 center of mass
    double m1 = 0.5 * h1 * w1 * ro;
    struct matrix c1 = triangle_centroid(p1->data, p3->data, p4.data);
    double d3c1 = vector_distance(p3, &c1);
    double Icm1 = I1 - (m1 * pow(d3c1, 2));
    // Moment of inertia around polygon center of mass
    double d1 = vector_distance(&c1, &o->centroid);
    double I1total = Icm1 + (m1 * pow(d1, 2)); 
    struct matrix v31 = matrix_subtract(p1, p3);
    struct matrix v34 = matrix_subtract(&p4, p3);
    double a1 = cross(v31.data, v34.data);
    if(a1 > 0) {
      I += I1total;
    } else {
      I -= I1total;
    }
    // Moment of inertia of triangle2 around p3
    double I2 = triangle_moment_of_inertia(h2, w2, ro);
    // Moment of inertia around traingle2 center of mass
    double m2 = 0.5 * h2 * w2 * ro;
    struct matrix c2 = triangle_centroid(p3->data, p4.data, p2->data);
    double d3c2 = vector_distance(p3, &c2);
    double Icm2 = I2 - (m2 * pow(d3c2, 2));
    // Moment of inertia around polygon center of mass
    double d2 = vector_distance(&c2, &o->centroid);
    double I2total = Icm2 + (m2 * pow(d2, 2));
    // Sum up moment of inertia 2
    struct matrix v32 = matrix_subtract(p2, p3);
    double a2 = cross(v34.data, v32.data);
    if(a2 > 0) {
      I += I2;
    } else {
      I -= I2;
    }
  }
  return fabs(I);
}

double triangle_moment_of_inertia(double h, double w, double ro) {
  return ro * (0.25 * (h * pow(w, 3)) + (1.0/12.0) * (pow(h, 3) * w));
}

double cross(double* p1, double* p2) {
  return p1[0] * p2[1] - p2[0] * p1[1];
}

double triangle_area_signed(double* p1, double* p2, double* p3) {
  double x1 = *(p1 + 0);
  double y1 = *(p1 + 1);
  double x2 = *(p2 + 0);
  double y2 = *(p2 + 1);
  double x3 = *(p3 + 0);
  double y3 = *(p3 + 1);
  return (x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1);
}

double triangle_area(struct matrix* p1, struct matrix* p2, struct matrix* p3) {
  struct matrix v12 = matrix_subtract(p2, p1);
  struct matrix v13 = matrix_subtract(p3, p1);
  struct matrix v13xv12 = matrix_cross_product(&v13, &v12);
  return 0.5 * matrix_norm_L2(&v13xv12);
}

struct matrix triangle_centroid(double* p1, double* p2, double* p3) {
  double cx = (1.0/3.0) * (p1[0] + p2[0] + p3[0]);
  double cy = (1.0/3.0) * (p1[0] + p2[0] + p3[0]);
  double data[3] = { cx, cy, 1.0 };
  struct matrix* c = vector_create(data, 3);
  return *c;
}

double triangle_area2(struct matrix* p1, struct matrix* p2, struct matrix* p3) {
  struct matrix v2 = matrix_subtract(p2, p1);
  struct matrix v3 = matrix_subtract(p3, p1);
  struct matrix proj23 = vector_projection(&v2, &v3);
  struct matrix p4 = matrix_add(p1, &proj23);
  printf("p4:\n");
  matrix_print(&p4);
  struct matrix v4 = matrix_subtract(&p4, p1);
  double x1 = matrix_value(&v4, 0, 0);
  double y1 = matrix_value(&v4, 1, 0);
  double x2 = matrix_value(&v3, 0, 0);
  double y2 = matrix_value(&v3, 1, 0);
  double a1 = x1 * y2 - x2 * y1;
  printf("v4:\n");
  matrix_print(&v4);
  struct matrix v42 = matrix_subtract(p2, &p4);
  struct matrix v43 = matrix_subtract(p3, &p4);
  x1 = matrix_value(&v42, 0, 0);
  y1 = matrix_value(&v42, 1, 0);
  x2 = matrix_value(&v43, 0, 0);
  y2 = matrix_value(&v43, 1, 0);
  double a2 = x1 * y2 - x2 * y1;
  printf("first area: %6.9f\n", a1); 
  printf("second area: %6.9f\n", a2);
  return 0.0;
}
