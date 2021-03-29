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

