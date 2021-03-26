#include "utils.h"
#include <stdlib.h>

double frand() {
  return (double)rand() / (double)RAND_MAX;
}

double random_double(double x_min, double x_max) {
  return x_min + (x_max - x_min) * frand();
}


void random_doubles(double x_min, double x_max, unsigned N, double* arr) {
  for(unsigned i = 0; i < N; i++) {
    arr[i] = random_double(x_min, x_max);
  }
}


void sort(double* arr, unsigned N) {
  
}
