#include <stdio.h>
#include "../linear_algebra.h"

int main(int argc, char* argv[])
{
  struct matrix* A = matrix_new(3, 3, 0.0);
  *matrix_element(A, 0, 0) = 1.0;
  *matrix_element(A, 0, 1) = 2.0;
  *matrix_element(A, 0, 2) = 3.0;
  *matrix_element(A, 1, 0) = 4.0;
  *matrix_element(A, 1, 1) = 5.0;
  *matrix_element(A, 1, 2) = 6.0;
  *matrix_element(A, 2, 0) = 7.0;
  *matrix_element(A, 2, 1) = 8.0;
  *matrix_element(A, 2, 2) = 9.0;
  struct matrix* I = matrix_new(3, 3, 0.0); 
  *matrix_element(I, 0, 0) = 1.0;
  *matrix_element(I, 1, 1) = 1.0;
  *matrix_element(I, 2, 2) = 1.0;
  matrix_print(A);
  matrix_print(I);
  struct matrix B = matrix_multiply(A, I);
  matrix_print(&B);
  return 0;
}
