#include "utils.h"
#include <stdlib.h>

double frand() {
  return (double)rand() / (double)RAND_MAX;
}

double random_double(double x_min, double x_max) {
  return x_min + (x_max - x_min) * frand();
}

int random_int(int x_min, int x_max) {
  return x_min + (x_max - x_min) * frand();
}

void random_doubles(double x_min, double x_max, unsigned N, double* arr) {
  for(unsigned i = 0; i < N; i++) {
    arr[i] = random_double(x_min, x_max);
  }
}

int imin(int a, int b) {
  return a < b ? a : b; 
}
int imax(int a, int b) {
  return a > b ? a : b;
}

double dmin(double a, double b) {
  return a < b ? a : b; 
}
double dmax(double a, double b) {
  return a > b ? a : b;
}

int dcomp(const void* _a, const void* _b) {
  double a = *((double*)_a); 
  double b = *((double*)_b); 
  return (a > b) - (a < b);
}

void dsort(double* arr, unsigned N) {
  qsort(arr, N, sizeof(double), dcomp);
}

int modulo(int x, int N) {
  return ((x % N) + N) % N;
}
