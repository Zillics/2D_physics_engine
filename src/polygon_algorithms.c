#include "polygon_algorithms.h"

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
