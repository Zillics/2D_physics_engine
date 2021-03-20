#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "../linear_algebra.h"

  START_TEST (test_matrix_multiplication)
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

