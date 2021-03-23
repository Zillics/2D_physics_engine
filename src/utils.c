#include "utils.h"
#include <stdlib.h>

double frand() {
  return (double)rand() / (double)RAND_MAX;
}

double random_double(double x_min, double x_max) {
  return x_min + (x_max - x_min) * frand();
}
