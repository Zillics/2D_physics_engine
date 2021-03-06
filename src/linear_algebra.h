#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

struct matrix {
  double* data;
  unsigned rows;
  unsigned cols;
};

struct matrix* matrix_new(unsigned rows, unsigned cols, double initVal);
struct matrix* vector_new(unsigned N, double initVal);
struct matrix* matrix_create(unsigned rows, unsigned cols, double data[cols][rows]);
struct matrix* vector_create(double* data, unsigned N);
struct matrix* vector_generate(struct matrix* min, struct matrix* max);
void matrix_delete(struct matrix* A);
void matrix_insert_col(struct matrix* A, double* x, unsigned i);
double* matrix_element(struct matrix* A, unsigned row, unsigned col);
double matrix_value(struct matrix* A, unsigned row, unsigned col);
double* matrix_col_raw(struct matrix* A, unsigned i);
struct matrix matrix_col(struct matrix* A, unsigned i);
void matrix_print(struct matrix* A);
size_t matrix_size(struct matrix* A);
void matrix_rotate(struct matrix* A, double deg);
void matrix_rotate_deg(struct matrix* A, double rad);
void matrix_rotate_rad(struct matrix* A, double rad);
void matrix_reset(struct matrix* A, double val);
double matrix_determinant_2D_raw(double* c1, double* c2);
double matrix_determinant_3D_raw(double* c1, double* c2, double* c3);
double matrix_determinant_2D(struct matrix* A);
double matrix_determinant_3D(struct matrix* A);
struct matrix matrix_transpose(struct matrix* A);
struct matrix matrix_negative(struct matrix* A);
struct matrix matrix_multiply(struct matrix* A, struct matrix* B); 
struct matrix matrix_multiply_scalar(struct matrix* A, double k);
struct matrix matrix_add(struct matrix* A, struct matrix* B);
struct matrix matrix_subtract(struct matrix* A, struct matrix* B);
struct matrix matrices_multiply(unsigned N, struct matrix matrices[N]); 
struct matrix matrix_cross_product(struct matrix* a, struct matrix* b);
struct matrix identity_matrix(unsigned dim);
struct matrix rotation_matrix_2D(double rad);
struct matrix translation_matrix_2D(double x, double y);
struct matrix matrix_rowwise_min(struct matrix* A);
struct matrix matrix_rowwise_max(struct matrix* A);
struct matrix matrix_colwise_min(struct matrix* A);
struct matrix matrix_colwise_max(struct matrix* A);
bool matrix_eq(struct matrix* A, struct matrix* B);
double matrix_sum(struct matrix* A);
double matrix_min(struct matrix* A);
double matrix_max(struct matrix* A);
double matrix_norm_L2(struct matrix* A);
double* vector_element(struct matrix* v, unsigned i);
double vector_value(struct matrix* v, unsigned i);
double vector_norm_L2_raw(double* v, unsigned N);
double vector_norm_L2(struct matrix* v);
double vector_norm(struct matrix* v);
struct matrix vector_subtract_raw(double* a, double* b, unsigned N);
double vector_dot(struct matrix* a, struct matrix* b);
double vector_dot_raw(double* a, double* b, unsigned N);
/** proj_b a, or projection of vector a on vector b */
struct matrix vector_projection(struct matrix* a, struct matrix* b);
struct matrix vector_normalize(struct matrix* v);
double vector_cross_2D(struct matrix* a, struct matrix* b);
double vector_cross_2D_raw(double* a, double* b);
double vector_angle_180(struct matrix* v1, struct matrix* v2);
double vector_angle_180_raw(double* a, double* b, unsigned N);
double vector_angle(struct matrix* v1, struct matrix* v2);
double vector_angle_raw(double* a, double* b);
double vector_angle_2D(struct matrix* v1, struct matrix* v2);
double vector_angle_2D_raw(double* a, double* b);
double vector_distance(struct matrix* v1, struct matrix* v2);
bool vector_lies_between(double* a, double* b, double* c);
int vector_orientation_raw(double* a, double* b, double* c);
bool vectors_counter_clockwise_2D_raw(double* a, double* b, double* c);
struct matrix unit_vector(struct matrix* v);
bool matrix_is_vector(struct matrix* v);

/** Determine whether two 2D lines intersect
 * @param a1
 * @param a2
 * @param b1
 * @param b2
 * @return true if line_a (a1->a2) and line_b (b1->b2) intersect, false otherwise 
 * */
bool lines_intersect_2D_raw(double* a1, double* a2, double* b1, double* b2);
void line_cut_end(double* p1, double* p2, unsigned N, double x);
#endif
