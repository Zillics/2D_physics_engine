#ifndef OBJECT_H
#define OBJECT_H
#include "polygon.h"

struct object {
  struct polygon shape;
  double angle;
  double pos;
  double vel;
  double acc;
};

#endif

