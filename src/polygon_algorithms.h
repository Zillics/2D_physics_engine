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



#endif
