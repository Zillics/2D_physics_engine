#ifndef POINT_H
#define POINT_H

struct point {
  unsigned x;
  unsigned y;
};

void print_point(struct point* p);
void print_points(struct point* p, unsigned n);

#endif
