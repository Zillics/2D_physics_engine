#include "polygon_algorithms.h"
#include "winding_number_algorithm.h"

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



void ear_clipping(struct polygon* o, struct polygon* ears) {
  unsigned N = polygon_nVertices(o);
  unsigned iEar = 0;
  unsigned remainingVertices = N;
  for(unsigned i = 0; i < remainingVertices; i++) {
    if(is_an_ear(o, i)) {
      unsigned vertix_idx[3] = { (i - 1) % N, i, (i + 1) % N };
      struct polygon* triangle = polygon_create_sub(o, 3, vertix_idx);
      ears[iEar] = *triangle;
      iEar += 1;
    }
  }
}

bool is_an_ear(struct polygon* o, int i2) {
  unsigned N = polygon_nVertices(o);
  int i1 = (i2-1) % N;
  int i3 = (i2+1) % N;
  // 1. Is angle between edge i-1 and i convex?
  if(polygon_edge_angle(o, i1, i2) > M_PI) {
    return false;
  }
  double points[3][2] =   { {polygon_vertex(o, i1)[0], polygon_vertex(o, i1)[1]},
                            {polygon_vertex(o, i2)[0], polygon_vertex(o, i2)[1]},
                            {polygon_vertex(o, i3)[0], polygon_vertex(o, i3)[1]}
                          };
  struct matrix* triangle = matrix_create(2, 3, points);
  // 2. Does triangle contain any of the polygon vertices?
  for(unsigned i = 0; i < N - 3; i++) {
    unsigned idx = i % N; 
    if(wn_PnPoly(polygon_vertex(o, idx), triangle, o->vertices.cols)) {
      return false;
    }
  }
  return true;
}
