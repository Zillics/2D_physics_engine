#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H
#include <stdio.h>
#include <math.h>

struct matrix {
  double* data;
  unsigned rows;
  unsigned cols;
};

struct matrix* matrix_new(unsigned rows, unsigned cols, double initVal);

void matrix_delete(struct matrix* A);

double* matrix_element(struct matrix* A, unsigned row, unsigned col);

double matrix_value(struct matrix* A, unsigned row, unsigned col);

double* matrix_col(struct matrix* A, unsigned i);

void matrix_print(struct matrix* A);

size_t matrix_size(struct matrix* A);

struct matrix matrix_multiply(struct matrix* A, struct matrix* B); 

struct matrix rotation_matrix(double rad);

#endif
