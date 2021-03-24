#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
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
  struct matrix* col = matrix_new(A->rows, 1, 0.0);
  for(unsigned i = 0; i < A->rows; i++) {
    *matrix_element(col, i, j) = matrix_value(A, i, j);
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
    ss += vector_norm_L2(matrix_col_raw(A, i), A->rows);
  }
  return ss;
}

double vector_norm_L2(double* v, unsigned N) {
  double ss = 0.0;
  for(unsigned i = 0; i < N; i++) {
    ss += pow(*(v + i), 2);
  }
  return sqrt(ss);
}

double vector_angle(struct matrix* v1, struct matrix* v2) {
  assert(v1->rows == 1 || v1->cols == 1);
  assert(v2->rows == 1 || v2->cols == 1);
  double v1norm = vector_norm_L2(v1->data, 2);
  double v2norm = vector_norm_L2(v2->data, 2);
  struct matrix v1v2 = matrix_multiply(v1, v2);
  double angle = acos(matrix_value(&v1v2, 0, 0) / (v1norm * v2norm));
  return angle;
}

struct matrix unit_vector(struct matrix* v) {
  assert(v->rows == 1 || v->cols ==1);
  unsigned N = v->rows * v->cols;
  double norm = vector_norm_L2(v->data, N);
  return matrix_multiply_scalar(v, 1.0/norm);
}

bool matrix_is_vector(struct matrix* v) {
  return v->cols == 1 || v->rows == 1;
}


struct matrix vector_projection(struct matrix* a, struct matrix* b) {
  struct matrix at = matrix_transpose(a);
  struct matrix bt = matrix_transpose(b);
  struct matrix aa = matrix_multiply(a, &at);
  struct matrix bb = matrix_multiply(b, &bt);
  double k = matrix_value(&aa, 0, 0) / matrix_value(&bb, 0, 0);
  return matrix_multiply_scalar(b, k);
}
