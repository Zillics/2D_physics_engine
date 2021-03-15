#include "point.h"
#include <stdio.h>

void print_point(struct point* p) {
  printf("(%d,%d)\n", p->x, p->y);
}

void print_points(struct point* ps, unsigned n) {
  for(unsigned i = 0; i < n; i++) {
    print_point(ps + i);
  }
}
