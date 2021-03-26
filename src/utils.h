#ifndef UTILS_H
#define UTILS_H


/** Returns random double in range of 0...1 */
double frand();

/** Returns random double in range of x_min...x_max */
double random_double(double x_min, double x_max);

/** Generate N random doubles and put them into arr */
void random_doubles(double x_min, double x_max, unsigned N, double* arr);

/** Sort array, consisting of N elements */
void sort(double* arr, unsigned N);

#endif
