#include "winding_number_algorithm.h"

static inline int isLeft( double* P0, double* P1, double* P2 )
{
    return ( (P1[0] - P0[0]) * (P2[1] - P0[1])
            - (P2[0] -  P0[0]) * (P1[1] - P0[1]) );
}

int wn_PnPoly( double* P, struct matrix* V, int n )
{
    int    wn = 0;    // the  winding number counter
    int i1 = 0;
    int i2 = 0;
    // loop through all edges of the polygon
    for (int i=0; i<n; i++) {   // edge from V[i] to  V[i+1]
      i1 = i % V->cols;
      i2 = (i + 1) % V->cols;
        if (matrix_value(V, 1, i1) <= P[1]) {          // start y <= P.y
            if (matrix_value(V, 1, i2)  > P[1]) {      // an upward crossing
                 if (isLeft( matrix_col_raw(V, i1), matrix_col_raw(V, i2), P) > 0) {  // P left of  edge
                     ++wn;            // have  a valid up intersect
                  }
            }
        } else {                        // start y > P.y (no test needed)
            if (matrix_value(V, 1, i2)  <= P[1]) {    // a downward crossing
                 if (isLeft( matrix_col_raw(V, i1), matrix_col_raw(V, i2), P) < 0) { // P right of  edge
                     --wn;            // have  a valid down intersect
                  }
            }
        }
    }
    return wn;
}

