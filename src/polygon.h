#ifndef POLYGON_H
#define POLYGON_H

#include <SDL2/SDL.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "linear_algebra.h"

/** Polygon with clock-wise ordered vertices. */
struct polygon {
  struct matrix vertices;
  struct matrix edge_midpoints;
  struct matrix edge_normals;
  struct matrix centroid;
  bool edge_normals_inward;
  double area;
};

struct polygon* new_square(int x, int y, unsigned width);
struct polygon* polygon_new(unsigned nVertices, double vertices[nVertices][2]);
struct polygon* polygon_new2(unsigned nVertices, struct matrix** vertices);
struct polygon* polygon_copy(struct polygon* o);
struct polygon* polygon_create_sub(struct polygon* o, unsigned nVertices, unsigned* vertix_idx);
/** Randomly generate a simple polygon of N vertices. */
struct polygon* polygon_generate(unsigned N, double r);
size_t polygon_size(struct polygon* o);
void polygon_delete(struct polygon* o);
void polygon_remove_vertex(struct polygon* p, unsigned i);
void polygon_insert_vertex(struct polygon* p, double* v, unsigned i);
void polygon_render(struct polygon* o, SDL_Renderer* renderer);
void polygon_render_vertex(struct polygon* o, SDL_Renderer* renderer, unsigned i1);
unsigned polygon_nVertices(struct polygon* o);
struct matrix polygon_vertex(struct polygon* o, unsigned i);
double* polygon_vertex_raw(struct polygon* o, unsigned i);
void polygon_edge_raw(struct polygon* o, unsigned i, double* p1, double* p2);
struct matrix polygon_edge(struct polygon* o, unsigned i);
/** 
 * @param o pointer to polygon
 * @param i1 index of edge 1
 * @param i2 index of edge 2
 * @return angle in radians between edge 1 and edge 2
 * */
double polygon_edge_angle(struct polygon* o, unsigned i1, unsigned i2);
/** Compute distance from polygon edge i to point p. */
double polygon_edge_distance(struct polygon* poly, unsigned i, double* p);
/** Find normal vector of vertix between point i and point i + 1. 
 * Set inward to true/false to specify whether normal points inward/outward. */
struct matrix polygon_edge_normal(struct polygon* o, unsigned i);
double* polygon_edge_normal_raw(struct polygon* o, unsigned i);
struct matrix polygon_edge_midpoint(struct polygon* o, unsigned i);
struct matrix polygon_compute_edge_normal(struct polygon* o, unsigned i, bool inward);
struct matrix polygon_compute_edge_midpoint(struct polygon* o, unsigned i);
bool polygon_eq(struct polygon* o1, struct polygon* o2);
void polygon_recompute_edge_normals(struct polygon* o, bool inward);
void polygon_recompute_edge_midpoints(struct polygon* o);
void polygon_centroid(struct polygon* o, double* x, double* y);
bool polygon_contains(struct polygon* o, double* p);
void polygon_rotate(struct polygon* o, double deg);
void polygon_translate(struct polygon* o, double* v, double k);
void polygon_place(struct polygon* o, double x, double y);
void polygon_rotate_rad(struct polygon* o, double rad);
void polygon_rotate_deg(struct polygon* o, double deg);
bool polygon_is_convex(struct polygon* o);
bool polygon_self_intersects(struct polygon* o);
bool vertices_intersect(struct matrix* verts); 
bool vertices_clockwise(struct matrix* verts);
/** Given polygon, populate n and c with normal and center of all vertices.
 * Set inward to true/false for normals pointing inwards/outwards. */
void polygon_vertex_normals(struct polygon* o, bool inward, struct matrix* n, struct matrix* c);
void polygon_print(struct polygon* o);
bool polygons_collide(struct polygon* o1, struct polygon* o2);
bool polygons_collide_penetration(struct polygon* o1, struct polygon* o2, struct matrix* penetration);
double polygon_area(struct polygon* o);
/** Compute polygon moment of inertia, given density ro. */
double polygon_moment_of_inertia(struct polygon* o, double ro);
double triangle_area(struct matrix* p1, struct matrix* p2, struct matrix* p3);
double triangle_area_signed(double* p1, double* p2, double* p3);
double triangle_area2(struct matrix* p1, struct matrix* p2, struct matrix* p3);
struct matrix triangle_centroid(double* p1, double* p2, double* p3);
double triangle_moment_of_inertia(double h, double w, double ro);
double cross(double* p1, double* p2);
#endif
