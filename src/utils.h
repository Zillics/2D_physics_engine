#ifndef UTILS_H
#define UTILS_H

struct matrix;

/** Returns random double in range of 0...1 */
double frand();

/** Returns random double in range of x_min...x_max */
double random_double(double x_min, double x_max);
int random_int(int x_min, int x_max);

int imin(int a, int b);
int imax(int a, int b);
double dmin(double a, double b);
double dmax(double a, double b);

/** Generate N random doubles and put them into arr */
void random_doubles(double x_min, double x_max, unsigned N, double* arr);

int modulo(int x, int N);

/** Sort array, consisting of N doubles */
void dsort(double* arr, unsigned N);
int dcomp(const void* _a, const void* _b);

void clockwise_sort(unsigned N, struct matrix** points);
int clockwise_comp(const void* _a, const void* _b);

#endif
