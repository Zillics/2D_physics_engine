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

struct matrix rotation_matrix(double rad) {
  struct matrix* R = matrix_new(2, 2, 0.0);
  *matrix_element(R, 0, 0) = cos(rad);
  *matrix_element(R, 0, 1) = -sin(rad);
  *matrix_element(R, 1, 0) = sin(rad);
  *matrix_element(R, 1, 1) = cos(rad); 
  return *R;
}

struct matrix matrix_multiply(struct matrix* A, struct matrix* B) {
  assert(A->cols == B->rows);
  struct matrix* ret = matrix_new(A->rows, B->cols, 0);
  for(unsigned i = 0; i < A->rows; i++) {
    for(unsigned j = 0; j < B->cols; j++) {
      double val = 0.0; 
      for(unsigned k = 0; k < A->rows; k++) {
        val += matrix_value(A, i, k) * matrix_value(B, k, j);
      }
      *matrix_element(ret, i, j) = val;
    }
  }
  return *ret;
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

