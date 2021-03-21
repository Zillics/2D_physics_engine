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

void matrix_rotate(struct matrix* A, double deg);

void matrix_rotate_deg(struct matrix* A, double rad);

void matrix_rotate_rad(struct matrix* A, double rad);

struct matrix matrix_transpose(struct matrix* A);

struct matrix matrix_negative(struct matrix* A);

struct matrix matrix_multiply(struct matrix* A, struct matrix* B); 

struct matrix matrix_multiply_scalar(struct matrix* A, double k); 

struct matrix matrices_multiply(unsigned N, struct matrix matrices[N]); 

struct matrix identity_matrix(unsigned dim);

struct matrix rotation_matrix_2D(double rad);

struct matrix translation_matrix_2D(double x, double y);

struct matrix matrix_rowwise_min(struct matrix* A);
struct matrix matrix_rowwise_max(struct matrix* A);

struct matrix matrix_colwise_min(struct matrix* A);
struct matrix matrix_colwise_max(struct matrix* A);

double matrix_min(struct matrix* A);
double matrix_max(struct matrix* A);

double matrix_norm_L2(struct matrix* A);

double vector_norm_L2(double* v, unsigned N);

double vector_angle(struct matrix* v1, struct matrix* v2);

struct matrix unit_vector(struct matrix* v);

#endif
