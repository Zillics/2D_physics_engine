#ifndef POLYGON_ALGORITHMS_H
#define POLYGON_ALGORITHMS_H

#include <stdbool.h>
#include "polygon.h"
#include "linear_algebra.h"

/** Separated axis theorem
 * 
 * @param o1 convex polygon
 * @param o2 convex polygon
 * @return true if polygons o1 and o2 overlap, false otherwise
 * */
bool separated_axis_theorem(struct polygon* o1, struct polygon* o2);
/** Helper function for separated axis theorem. 
 * @param o1 convex polygon
 * @param o2 convex polygon
 * @return true if all projected edges of o1 and o2 on the normals of o1 overlap, false otherwise. */
bool polygon_axes_overlap(struct polygon* o1, struct polygon* o2);

/** Hertel-Melhorn polygon partitioning
 * Partition polygon into several convex sub-polygons.
 * */
void hertel_melhorn(struct polygon* o);

/** Ear clipping polygon triangulation.
 * Partition polygon into n triangels consisting of vertices
 * that are suitable to form ears.
 * @param o simple polygon
 * @param ears preallocated container which will be populated with the ears
 * */
void ear_clipping(struct polygon* o, struct polygon* ears, unsigned* nEars);

/** Is triangle consisting of vertices i-1, i, i+1 an ear of polygon?
 * @param o simple polygon
 * @param i index of middle vertix
 * @return true if it is an ear, false otherwise
 * */
bool is_an_ear(struct polygon* o, int i);

/** Gilbert–Johnson–Keerthi distance algorithm */
bool GJK(struct polygon* o1, struct polygon* o2);
bool handle_simplex(struct matrix* simplex[3], struct matrix* d);
bool line_case(struct matrix* p1, struct matrix* p2, struct matrix* d);
bool triangle_case(struct matrix* p1, struct matrix* p2, struct matrix* d);

/** */
struct matrix furthest_point(struct polygon* o, struct matrix* dir);
struct matrix support_point(struct polygon* o1, struct polygon* o2, struct matrix* direction);

/** Given point p1, p2 and direction vector d does 
 * p2 lie on the other side of origin? */
bool passes_origin(struct matrix* p1, struct matrix* p2);

/** Given line segment through points p1 and p2,
 * find the line's normal n directed towards origin. */
struct matrix normal_towards_origin(struct matrix* p1, struct matrix* p2);

struct matrix triple_cross_product(struct matrix* A, struct matrix* B, struct matrix* C);

#endif
