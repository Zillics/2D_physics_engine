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
void ear_clipping(struct polygon* o, struct polygon* ears);

/** Is triangle consisting of vertices i-1, i, i+1 an ear of polygon?
 * @param o simple polygon
 * @param i index of middle vertix
 * @return true if it is an ear, false otherwise
 * */
bool is_an_ear(struct polygon* o, int i);

#endif
