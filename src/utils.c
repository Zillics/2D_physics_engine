#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "linear_algebra.h"

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

void clockwise_sort(unsigned N, struct matrix** points) {
  // Compute centroid
  double cx = 0.0;
  double cy = 0.0;
  for(unsigned i = 0; i < N; i++) {
    cx += points[i]->data[0];
    cy += points[i]->data[1];
  }
  cx = cx / (double)N;
  cy = cy / (double)N;
  // Move points s.t. centroid is at oribin
  for(unsigned i = 0; i < N; i++) {
    points[i]->data[0] -= cx;
    points[i]->data[1] -= cy;
  }
  // Sort points
  //size_t elem_size = matrix_size(points[0]);
  size_t elem_size = sizeof(struct matrix*);
  qsort(points, N, elem_size, clockwise_comp);
  // Move points back
  for(unsigned i = 0; i < N; i++) {
    points[i]->data[0] += cx;
    points[i]->data[1] += cy;
  }
}


int clockwise_comp(const void* _a, const void* _b) {
  struct matrix* a = *(struct matrix**)_a;
  struct matrix* b = *(struct matrix**)_b;
  double ax = a->data[0];
  double ay = a->data[1];
  double bx = b->data[0];
  double by = b->data[1];
  int ret = 1;
  if (ax >= 0 && bx < 0) {
    return ret;
  }
  if (ax < 0 && bx >= 0) {
    return -ret;
  }
  if (ax == 0 && bx == 0) {
    if (ay >= 0 || by >= 0) {
      return ay > by ? ret : -ret;
    }
    return by > ay ? ret : -ret;
  }

  // compute the cross product of vectors (center -> a) x (center -> b)
  int det = ax * by - bx * ay;
  if (det < 0) {
    return ret;
  }
  if (det > 0) {
    return -ret;
  }
  // points a and b are on the same line from the center
  // check which point is closer to the center
  int d1 = ax * ax + ay * ay;
  int d2 = bx * bx + by * by;
  return d1 > d2 ? ret : -ret;
}


int clockwise_comp2(const void* _a, const void* _b) {
  struct matrix* a = *(struct matrix**)_a;
  struct matrix* b = *(struct matrix**)_b;
  double ax = a->data[0];
  double ay = a->data[1];
  double bx = b->data[0];
  double by = b->data[1];
  //int det = (*a[0]) * (*b[1]) - (*b[0]) * (*a[1]);
  double det = (ax * by) - (bx * ay);
  printf("a: (%.4f, %.4f), b: (%.4f, %.4f) -> %.4f \n", ax, ay, bx, by, det); 
  return -(int)det;

}
