#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "linear_algebra.h"

struct matrix* matrix_new(unsigned rows, unsigned cols, double initVal) {
  struct matrix* A = malloc(sizeof(struct matrix));
  A->data = malloc(rows * cols * sizeof(double));
  for(unsigned i = 0; i < rows * cols; i++) {
    A->data[i] = initVal;
  }
  A->rows = rows;
  A->cols = cols;
  return A;
}

void matrix_delete(struct matrix* A) {
  free(A->data);
}

struct matrix* matrix_create(unsigned rows, unsigned cols, double data[cols][rows]) {
  struct matrix* A = malloc(sizeof(struct matrix));
  A->data = malloc(rows * cols * sizeof(double));
  A->rows = rows;
  A->cols = cols;
  for(unsigned i = 0; i < rows; i++) {
    for(unsigned j = 0; j < cols; j++) {
      *matrix_element(A, i, j) = data[j][i];
    }
  }
  return A;
}

struct matrix* vector_new(unsigned N, double initVal) {
  return matrix_new(N, 1, initVal);
}

struct matrix* vector_create(double* data, unsigned N) {
  struct matrix* v = malloc(sizeof(struct matrix));
  v->data = malloc(N * sizeof(double));
  for(unsigned i = 0; i < N; i++) {
    v->data[i] = data[i];
  }
  v->rows = N;
  v->cols = 1;
  return v;
}

struct matrix* vector_generate(struct matrix* min, struct matrix* max) {
  assert(matrix_is_vector(min));
  assert(matrix_is_vector(max));

  unsigned N = min->rows * min->cols;
  assert(N == max->rows * max->cols);
  struct matrix* v = vector_new(N, 0.0);
  for(unsigned i = 0; i < N; i++) {
    *vector_element(v, i) = random_double(vector_value(min, i), vector_value(max, i));
  }
  return v;
}

void matrix_insert_col(struct matrix* A, double* x, unsigned j) {
  assert(j < A->cols);
  for(unsigned i = 0; i < A->rows; i++) {
    *(A->data + j*A->rows + i) = *(x + i);
  }
}

double* matrix_element(struct matrix* A, unsigned row, unsigned col) {
  assert(row < A->rows);
  assert(col < A->cols);
  return A->data + col * A->rows + row;
}

double matrix_value(struct matrix* A, unsigned row, unsigned col) {
  assert(row < A->rows);
  assert(col < A->cols);
  return *(matrix_element(A, row, col));
}

struct matrix matrix_col(struct matrix* A, unsigned j) {
  assert(j < A->cols);
  unsigned N = A->rows;
  struct matrix* col = vector_new(N, 0.0);
  for(unsigned i = 0; i < N; i++) {
    *vector_element(col, i) = matrix_value(A, i, j);
  }
  return *col;
}

double* matrix_col_raw(struct matrix* A, unsigned i) {
  assert(i < A->cols);
  return A->data + i * A->rows;
}

struct matrix identity_matrix(unsigned dim) {
  struct matrix* I = matrix_new(dim, dim, 0.0);
  for(unsigned i = 0; i < dim; i++) {
    *matrix_element(I, i, i) = 1.0;
  }
  return *I;
}

void matrix_rotate(struct matrix* A, double deg) {
  matrix_rotate_deg(A, deg);
}

void matrix_rotate_deg(struct matrix* A, double deg) {
  double rad = (deg / 360) * (2 * M_PI);
  matrix_rotate_rad(A, rad);
}

void matrix_rotate_rad(struct matrix* A, double rad) {
  assert(A->rows == 3);
  struct matrix R = rotation_matrix_2D(rad);
  *A = matrix_multiply(&R, A);
}

void matrix_reset(struct matrix* A, double val) {
  unsigned N = A->rows * A->cols;
  for(unsigned i = 0; i < N; i++) {
    *(A->data + i) = val;
  }
}

struct matrix rotation_matrix_2D(double rad) {
  struct matrix* R = matrix_new(3, 3, 0.0);
  *matrix_element(R, 0, 0) = cos(rad);
  *matrix_element(R, 0, 1) = -sin(rad);
  *matrix_element(R, 1, 0) = sin(rad);
  *matrix_element(R, 1, 1) = cos(rad); 
  *matrix_element(R, 2, 2) = 1.0;
  return *R;
}

struct matrix translation_matrix_2D(double x, double y) {
  struct matrix I = identity_matrix(3);
  *matrix_element(&I, 0, 2) = x;
  *matrix_element(&I, 1, 2) = y;
  return I;
}

double matrix_determinant_3D_raw(double* c1, double* c2, double* c3) {
  return c1[0] * c2[1] * c3[2] + c2[0] * c3[1] * c1[2] + c3[0] * c1[1] * c2[2]
        -(c3[0] * c2[1] * c1[2]) -(c2[0] * c1[1] * c3[2]) -(c1[0] * c3[1] * c2[2]);
}

double matrix_determinant_2D_raw(double* c1, double* c2) {
  return c1[0] * c2[1] - (c2[0] * c1[1]);
}

double matrix_determinant_2D(struct matrix* A) {
  return matrix_determinant_2D_raw(matrix_col_raw(A, 0), matrix_col_raw(A, 1));
}

double matrix_determinant_3D(struct matrix* A) {
  return matrix_determinant_3D_raw(matrix_col_raw(A, 0), matrix_col_raw(A, 1), matrix_col_raw(A, 2));
}

struct matrix matrix_transpose(struct matrix* A) {
  struct matrix* At = matrix_new(A->cols, A->rows, 0.0);
  for(unsigned i = 0; i < A->rows; i++) {
    for(unsigned j = 0; j < A->cols; j++) {
      *matrix_element(At, j, i) = matrix_value(A, i, j);
    }
  }
  return *At;
}

struct matrix matrix_negative(struct matrix* A) {
  struct matrix* ret = matrix_new(A->rows, A->cols, 0.0);
  for(unsigned i = 0; i < A->rows; i++) {
    for(unsigned j = 0; j < A->cols; j++) {
      *matrix_element(ret, i, j) = -matrix_value(A, i, j);
    }
  }
  return *ret;
}


struct matrix matrix_add(struct matrix* A, struct matrix* B) {
  struct matrix* C = matrix_new(A->rows, A->cols, 0.0);
  unsigned N = A->rows * A->cols;
  assert(N == B->rows * B->cols);
  for(unsigned i = 0; i < N; i++) {
    *(C->data + i) = *(A->data + i) + *(B->data + i);
  }
  return *C;
}

struct matrix matrix_subtract(struct matrix* A, struct matrix* B) {
  struct matrix* C = matrix_new(A->rows, A->cols, 0.0);
  unsigned N = A->rows * A->cols;
  assert(N == B->rows * B->cols);
  for(unsigned i = 0; i < N; i++) {
    *(C->data + i) = *(A->data + i) - *(B->data + i);
  }
  return *C;
}

struct matrix matrix_multiply(struct matrix* A, struct matrix* B) {
  assert(A->cols == B->rows);
  struct matrix* ret = matrix_new(A->rows, B->cols, 0);
  for(unsigned i = 0; i < A->rows; i++) {
    for(unsigned j = 0; j < B->cols; j++) {
      double val = 0.0; 
      for(unsigned k = 0; k < A->cols; k++) {
        val += matrix_value(A, i, k) * matrix_value(B, k, j);
      }
      *matrix_element(ret, i, j) = val;
    }
  }
  return *ret;
}

struct matrix matrix_multiply_scalar(struct matrix* A, double k) {
  struct matrix* ret = matrix_new(A->rows, A->cols, 0.0);
  for(unsigned i = 0; i < A->rows; i++) {
    for(unsigned j = 0; j < A->cols; j++) {
      *matrix_element(ret, i, j) = matrix_value(A, i, j) * k;
    }
  }
  return *ret;
}

struct matrix matrices_multiply(unsigned N, struct matrix matrices[N]) {
  assert(N > 1);
  struct matrix ret = matrices[0];
  for(unsigned i = 1; i < N; i++) {
    ret = matrix_multiply(&ret, &matrices[i]);
  }
  return ret;
}

struct matrix matrix_cross_product(struct matrix* a, struct matrix* b) {
  assert(matrix_is_vector(a));
  assert(matrix_is_vector(b));
  unsigned N = a->rows * a->cols;
  assert(N == b->rows * b->cols);
  struct matrix* axb = matrix_new(N, 1, 0.0);
  for(unsigned i = 0; i < N; i++) {
    unsigned i1 = (N - 2 + i) % N;
    unsigned i2 = (N - 1 + i) % N;
    *matrix_element(axb, i, 0) += *(a->data + i1) * (*(b->data + i2));
    *matrix_element(axb, i, 0) -= *(a->data + i2) * (*(b->data + i1));
  }
  return *axb;
}

size_t matrix_size(struct matrix* A) {
  return sizeof(struct matrix) + A->rows * A->cols * sizeof(double);
}

void matrix_print(struct matrix* A) {
  for(unsigned i = 0; i < A->rows; i++) {
    for(unsigned j = 0; j < A->cols; j++) {
      printf("%6.4f", matrix_value(A, i, j));
      if(j < A->cols - 1) {
        printf("\t");
      }
    }
    printf("\n");
  }
  printf("---------------\n");
}

struct matrix matrix_rowwise_min(struct matrix* A) {
  struct matrix* ret = matrix_new(A->rows, 1, 0.0);
  double val;
  for(unsigned i = 0; i < A->rows; i++) {
    double min = matrix_value(A, i, 0);
    for(unsigned j = 1; j < A->cols; j++) {
      val = matrix_value(A, i, j);
      if(val < min) {
        min = val;
      }
    }
      *matrix_element(ret, i, 0) = min;
  }
  return *ret;
}

struct matrix matrix_rowwise_max(struct matrix* A) {
  struct matrix* ret = matrix_new(A->rows, 1, 0.0);
  double val;
  for(unsigned i = 0; i < A->rows; i++) {
    double min = matrix_value(A, i, 0);
    for(unsigned j = 1; j < A->cols; j++) {
      val = matrix_value(A, i, j);
      if(val > min) {
        min = val;
      }
    }
      *matrix_element(ret, i, 0) = min;
  }
  return *ret;
}

struct matrix matrix_colwise_min(struct matrix* A) {
  struct matrix* ret = matrix_new(A->cols, 1, 0.0);
  double val;
  for(unsigned j = 0; j < A->cols; j++) {
    double min = matrix_value(A, 0, j);
    for(unsigned i = 1; i < A->cols; i++) {
      val = matrix_value(A, i, j);
      if(val < min) {
        min = val;
      }
    }
      *matrix_element(ret, j, 0) = min;
  }
  return *ret;
}

struct matrix matrix_colwise_max(struct matrix* A) {
  struct matrix* ret = matrix_new(A->cols, 1, 0.0);
  double val;
  for(unsigned j = 0; j < A->cols; j++) {
    double min = matrix_value(A, 0, j);
    for(unsigned i = 1; i < A->cols; i++) {
      val = matrix_value(A, i, j);
      if(val > min) {
        min = val;
      }
    }
      *matrix_element(ret, j, 0) = min;
  }
  return *ret;
}

double matrix_sum(struct matrix* A) {
  double sum = 0.0;
  for(unsigned i = 0; i < A->cols * A->rows; i++) {
    sum += *(A->data + i);
  }
  return sum;
}

double matrix_min(struct matrix* A) {
  double min = A->data[0];
  for(unsigned k = 1; k < A->cols * A->rows; k++) {
    if(A->data[k] < min) {
      min = A->data[k];
    }
  }
  return min;
}

double matrix_max(struct matrix* A) {
  double max = A->data[0];
  for(unsigned k = 1; k < A->cols * A->rows; k++) {
    if(A->data[k] > max) {
      max = A->data[k];
    }
  }
  return max;
}

double matrix_norm_L2(struct matrix* A) {
  double ss = 0.0;
  for(unsigned i = 0; i < A->cols; i++) {
    ss += vector_norm_L2_raw(matrix_col_raw(A, i), A->rows);
  }
  return ss;
}

double* vector_element(struct matrix* v, unsigned i) {
  return v->data + i;
}

double vector_value(struct matrix* v, unsigned i) {
  return *(v->data + i);
}

struct matrix vector_subtract_raw(double* a, double* b, unsigned N) {
  struct matrix* c = vector_new(N, 0.0);
  for(unsigned i = 0; i < N; i++) {
    *vector_element(c, i) = *(a + i) - *(b + i);
  }
  return *c;
}

double vector_dot(struct matrix* a, struct matrix* b) {
  assert(matrix_is_vector(a));
  assert(matrix_is_vector(b));
  unsigned N = a->rows * a->cols;
  assert(N == b->rows * b->cols);
  return vector_dot_raw(a->data, b->data, N);
}

double vector_dot_raw(double* a, double* b, unsigned N) {
  double ret = 0.0;
  for(unsigned i = 0; i < N; i++) {
    ret += *(a + i) * (*(b + i));
  }
  return ret;
}

double vector_norm_L2_raw(double* v, unsigned N) {
  double ss = 0.0;
  for(unsigned i = 0; i < N; i++) {
    ss += pow(*(v + i), 2);
  }
  return sqrt(ss);
}

double vector_norm_L2(struct matrix* v) {
  assert(matrix_is_vector(v));
  return vector_norm_L2_raw(v->data, v->rows * v->cols);
}

double vector_norm(struct matrix* v) {
  return vector_norm_L2(v);
}

double vector_angle_180(struct matrix* a, struct matrix* b) {
  assert(matrix_is_vector(a));
  assert(matrix_is_vector(b));
  unsigned N = a->rows * a->cols;
  assert(N == b->rows * b->cols);
  return vector_angle_180_raw(a->data, b->data, N);
}

double vector_angle_180_raw(double* a, double* b, unsigned N) {
  double ab = vector_dot_raw(a, b, N);
  double _ab_ = vector_norm_L2_raw(a, N) * vector_norm_L2_raw(b, N);
  return acos(ab / (_ab_));
}

double vector_angle_2D(struct matrix* a, struct matrix* b) {
  return vector_angle_2D_raw(a->data, b->data);  
}

double vector_angle_2D_raw(double* a, double* b) {
  double cross = vector_cross_2D_raw(a, b);
  if(cross <= 0.0) {
    return vector_angle_180_raw(a, b, 2);
  } else {
      return 2 * M_PI - vector_angle_180_raw(a, b, 2);
  }
}

double vector_angle(struct matrix* a, struct matrix* b) {
  return vector_angle_2D(a, b);
}

double vector_angle_raw(double* a, double* b) {
  return vector_angle_2D_raw(a, b);
}

double vector_cross_2D(struct matrix* a, struct matrix* b) {
  assert(matrix_is_vector(a));
  assert(matrix_is_vector(b));
  return vector_cross_2D_raw(a->data, b->data);
}

double vector_cross_2D_raw(double* a, double* b) {
  return a[0] * b[1] - a[1] * b[0];
}

struct matrix unit_vector(struct matrix* v) {
  assert(v->rows == 1 || v->cols ==1);
  unsigned N = v->rows * v->cols;
  double norm = vector_norm_L2(v);
  return matrix_multiply_scalar(v, 1.0/norm);
}

bool matrix_is_vector(struct matrix* v) {
  return v->cols == 1 || v->rows == 1;
}

struct matrix vector_projection(struct matrix* a, struct matrix* b) {
  // a1 = ((a . b) / (b . b))b
  double ab = vector_dot(a, b);
  double bb = vector_dot(b, b);
  return matrix_multiply_scalar(b, ab/bb);
}

double vector_distance(struct matrix* v1, struct matrix* v2) {
  assert(matrix_is_vector(v1));
  assert(matrix_is_vector(v2));
  struct matrix diff = matrix_subtract(v1, v2);
  return vector_norm_L2(&diff);
}

int vectors_orientation_raw(double* a, double* b, double* c) {
  double tol = 1e-6;
  double val = (b[1] - a[1]) * (c[0] - b[0]) - (c[1] - b[1]) * (b[0] - a[0]);
  // counter-clockwise
  if(val < -tol) {
    return -1;
  }
  // clockwise
  if(val > tol) {
    return 1;
  }
  // colinear
  return 0;
}

bool vector_lies_between(double* a, double* b, double* c) {
  return (b[0] < fmax(a[0], c[0])) && (b[0] > fmin(a[0], c[0]))
      && (b[1] < fmax(a[1], c[1])) && (b[1] > fmin(a[1], c[1]));
}

bool vectors_counter_clockwise_2D_raw(double* a, double* b, double* c) {
  return vectors_orientation_raw(a, b, c) < 0;
}

/*
bool lines_intersect_2D_raw(double* a1, double* a2, double* b1, double* b2) {
  printf("a1: (%.4f, %.4f), a2: (%.4f, %.4f), b1: (%.4f, %.4f), b2: (%.4f, %.4f)\n", a1[0], a1[1], a2[0], a2[1], b1[0], b1[1], b2[0], b2[1]);
  if(vectors_counter_clockwise_2D_raw(a1, a2, b1) == vectors_counter_clockwise_2D_raw(a1, a2, b2)) {
    return false;
  }
  if(vectors_counter_clockwise_2D_raw(b1, b2, a1) == vectors_counter_clockwise_2D_raw(b1, b2, a2)) {
    return false;
  }
  return true;
}

*/

void line_cut_end(double* p1, double* p2, unsigned N, double x) {
  for(unsigned i = 0; i < N; i++) {
    double dir;
    if(p2[i] - p1[i] > 0) {
      dir = 1.0;
    } else {
      dir = -1.0;
    }
    p1[i] += dir * x;
    p2[i] -= dir * x;
  }
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool lines_intersect_2D_raw(double* a1_, double* a2_, double* b1_, double* b2_)
{
  double a1[2] = { a1_[0], a1_[1] };
  double a2[2] = { a2_[0], a2_[1] };
  double b1[2] = { b1_[0], b1_[1] };
  double b2[2] = { b2_[0], b2_[1] };
  line_cut_end(a1, a2, 2, 1e-6);
  line_cut_end(b1, b2, 2, 1e-6);
  printf("a1: (%.4f, %.4f), a2: (%.4f, %.4f), b1: (%.4f, %.4f), b2: (%.4f, %.4f)\n", a1[0], a1[1], a2[0], a2[1], b1[0], b1[1], b2[0], b2[1]);
  // Find the four orientations needed for general and
  // special cases
  int o1 = vectors_orientation_raw(a1, a2, b1);
  int o2 = vectors_orientation_raw(a1, a2, b2);
  int o3 = vectors_orientation_raw(b1, b2, a1);
  int o4 = vectors_orientation_raw(b1, b2, a2);

  // General case
  if (o1 != o2 && o3 != o4) {
  printf("A\n");
  return true;
  }

  // Special Cases§
  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
  if (o1 == 0 && vector_lies_between(a1, b1, a2)) {
  printf("B\n");
  return true;
  }

  // p1, q1 and q2 are colinear and q2 lies on segment p1q1
  if (o2 == 0 && vector_lies_between(a1, b2, a2)) {
  printf("C\n");
  return true;
  }

  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
  if (o3 == 0 && vector_lies_between(b1, a1, b2)) {
  printf("D\n");
  return true;
  }

  // p2, q2 and q1 are colinear and q1 lies on segment p2q2
  if (o4 == 0 && vector_lies_between(b1, a2, b2)) {
  printf("E\n");
  return true;
  }

  return false; // Doesn't fall in any of the above cases
}
