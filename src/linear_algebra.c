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


double* matrix_col(struct matrix* A, unsigned i) {
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

struct matrix matrices_multiply(unsigned N, struct matrix matrices[N]) {
  assert(N > 1);
  struct matrix ret = matrices[0];
  for(unsigned i = 1; i < N; i++) {
    ret = matrix_multiply(&ret, &matrices[i]);
  }
  return ret;
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
    ss += vector_norm_L2(matrix_col(A, i), A->rows);
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
