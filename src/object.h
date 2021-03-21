#ifndef OBJECT_H
#define OBJECT_H
#include "polygon.h"
#include "linear_algebra.h"

struct object {
  struct polygon shape;
  double mass;
  struct matrix vel;
  struct matrix acc;
  struct matrix angular_vel;
  struct matrix angular_acc;
};

#endif

