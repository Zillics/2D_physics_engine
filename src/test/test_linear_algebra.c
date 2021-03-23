#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "../linear_algebra.h"

  START_TEST (test_matrix_multiplication) {
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
    struct matrix B = matrix_multiply(A, I);
    ck_assert_double_eq(matrix_value(A, 0, 0), matrix_value(&B, 0, 0));
    ck_assert_double_eq(matrix_value(A, 0, 1), matrix_value(&B, 0, 1));
    ck_assert_double_eq(matrix_value(A, 0, 2), matrix_value(&B, 0, 2));
    ck_assert_double_eq(matrix_value(A, 1, 0), matrix_value(&B, 1, 0));
    ck_assert_double_eq(matrix_value(A, 1, 1), matrix_value(&B, 1, 1));
    ck_assert_double_eq(matrix_value(A, 1, 2), matrix_value(&B, 1, 2));
    ck_assert_double_eq(matrix_value(A, 2, 0), matrix_value(&B, 2, 0));
    ck_assert_double_eq(matrix_value(A, 2, 1), matrix_value(&B, 2, 1));
    ck_assert_double_eq(matrix_value(A, 2, 2), matrix_value(&B, 2, 2));

    struct matrix* C = matrix_new(2, 6, 1.0);
    struct matrix D = matrix_transpose(C);
    struct matrix CD = matrix_multiply(C, &D);
    ck_assert_int_eq(CD.rows, 2);
    ck_assert_int_eq(CD.cols, 2);
    struct matrix* v = matrix_new(1, 6, 1.0);
    struct matrix* X = matrix_new(6, 8, 1.0);
    struct matrix vX = matrix_multiply(v, X);
    ck_assert_int_eq(vX.rows, 1);
    ck_assert_int_eq(vX.cols, 8);

    struct matrix vt = matrix_transpose(v);
    struct matrix Xt = matrix_transpose(X);
    struct matrix Xtvt = matrix_multiply(&Xt, &vt);
    ck_assert_int_eq(Xtvt.rows, 8);
    ck_assert_int_eq(Xtvt.cols, 1);
  }
  END_TEST

  START_TEST (test_matrix_min_max) {
    struct matrix* A = matrix_new(3, 5, 0.0);
    *matrix_element(A, 0, 0) = 500.0;
    *matrix_element(A, 0, 1) = 1000.0;
    *matrix_element(A, 0, 2) = 93.0;
    *matrix_element(A, 0, 3) = -123.0; // <-----------
    *matrix_element(A, 0, 4) = 0.0;

    *matrix_element(A, 1, 0) = 0.0; // <---------
    *matrix_element(A, 1, 1) = 1.0;
    *matrix_element(A, 1, 2) = 2.0;
    *matrix_element(A, 1, 3) = 3.0;
    *matrix_element(A, 1, 4) = 4.0;

    *matrix_element(A, 2, 0) = 1.0; 
    *matrix_element(A, 2, 1) = 2.0;
    *matrix_element(A, 2, 2) = -0.5; // <---------
    *matrix_element(A, 2, 3) = 0.0;
    *matrix_element(A, 2, 4) = 3.0;

    struct matrix min = matrix_rowwise_min(A);
    struct matrix max = matrix_rowwise_max(A);
    ck_assert_double_eq(matrix_value(&min, 0, 0), -123.0);
    ck_assert_double_eq(matrix_value(&min, 1, 0), 0.0);
    ck_assert_double_eq(matrix_value(&min, 2, 0), -0.5);

    ck_assert_double_eq(matrix_value(&max, 0, 0), 1000.0);
    ck_assert_double_eq(matrix_value(&max, 1, 0), 4.0);
    ck_assert_double_eq(matrix_value(&max, 2, 0), 3.0);

    double min_val = matrix_min(A);
    double max_val = matrix_max(A);

    ck_assert_double_eq(min_val, -123.0);
    ck_assert_double_eq(max_val, 1000.0);
  }
  END_TEST

  START_TEST (test_matrix_transpose) {
    struct matrix* v = matrix_new(5, 1, 0.0);
    *matrix_element(v, 0, 0) = 1.0;
    *matrix_element(v, 1, 0) = 2.0;
    *matrix_element(v, 2, 0) = 3.0;
    *matrix_element(v, 3, 0) = 4.0;
    *matrix_element(v, 4, 0) = 5.0;
    struct matrix vt = matrix_transpose(v);
    ck_assert_int_eq(v->rows, vt.cols);
    ck_assert_int_eq(v->cols, vt.rows);
    ck_assert_double_eq(matrix_value(v, 0, 0), matrix_value(&vt, 0, 0));
    ck_assert_double_eq(matrix_value(v, 1, 0), matrix_value(&vt, 0, 1));
    ck_assert_double_eq(matrix_value(v, 2, 0), matrix_value(&vt, 0, 2));
    ck_assert_double_eq(matrix_value(v, 3, 0), matrix_value(&vt, 0, 3));
    ck_assert_double_eq(matrix_value(v, 4, 0), matrix_value(&vt, 0, 4));
  }
  END_TEST

  START_TEST (test_cross_product) {
    struct matrix* a = matrix_new(3, 1, 0);
    struct matrix* b = matrix_new(3, 1, 0);
    double a_data[3] = { 2.0, 4.5, 1.24 };
    double b_data[3] = { -1.32, 5.0, 2.22 };
    matrix_insert_col(a, a_data, 0);
    matrix_insert_col(b, b_data, 0);
    struct matrix axb = matrix_cross_product(a, b);
    ck_assert_double_eq_tol(matrix_value(&axb, 0, 0), 3.79, 1e-6); 
    ck_assert_double_eq_tol(matrix_value(&axb, 1, 0), -6.0768, 1e-6); 
    ck_assert_double_eq_tol(matrix_value(&axb, 2, 0), 15.94, 1e-6);
  }
  END_TEST

Suite* matrix_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Matrix");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_matrix_multiplication);
    tcase_add_test(tc_core, test_matrix_min_max);
    tcase_add_test(tc_core, test_matrix_transpose);
    tcase_add_test(tc_core, test_cross_product);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = matrix_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

