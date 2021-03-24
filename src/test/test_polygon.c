#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "../polygon.h"
#include "../linear_algebra.h"
#include "../utils.h"

START_TEST(test_triangle_area) {
  struct matrix* p1 = matrix_new(3, 1, 0.0);
  struct matrix* p2 = matrix_new(3, 1, 0.0);
  struct matrix* p3 = matrix_new(3, 1, 0.0);
  double p1_data[3] = {0.0,0.0,1.0};
  double p2_data[3] = {0.0,2.0,1.0};
  double p3_data[3] = {4.0,0.0,1.0};
  matrix_insert_col(p1, p1_data, 0);
  matrix_insert_col(p2, p2_data, 0);
  matrix_insert_col(p3, p3_data, 0);
  double A_act = 0.5 * 2.0 * 4.0;
  double A = triangle_area(p1, p2, p3);
  ck_assert_double_eq(A, A_act);
  for(double rad = 0.0; rad < 2*M_PI; rad += (2*M_PI)*0.001) {
    matrix_rotate_rad(p1, rad);
    matrix_rotate_rad(p2, rad);
    matrix_rotate_rad(p3, rad);
    A = triangle_area(p1, p2, p3);
    ck_assert_double_eq_tol(A, A_act, 1e-6);
  }
}
END_TEST

START_TEST(test_polygon_area) {
  struct color color = { .r=255, .g=255, .b=255 };
  // 10 * 2 square
  double p[4][2] = {{0.0, 2.0},
                    {10.0, 2.0},
                    {10.0, 0.0},
                    {0.0, 0.0}};
  struct polygon* o = polygon_new(4, p, color);
  double A_act = 10.0 * 2.0;
  double A = polygon_area(o);
  ck_assert_double_eq_tol(A, A_act, 1e-6);
  for(double rad = 0.0; rad < 2*M_PI; rad += (2*M_PI)*0.1) {
    polygon_rotate_rad(o, rad);
    for(unsigned i = 0; i < 10; i++) {
      double shift[2] = { random_double(-1.0, 1.0), random_double(-1.0, 1.0) };
      polygon_translate(o, shift, 1.0);
      A = polygon_area(o);
      ck_assert_double_eq_tol(A, A_act, 1e-6);
    } 
  }
}
END_TEST

START_TEST(test_polygon_inertia) {
  struct color color = { .r=255, .g=255, .b=255 };
  // 10 * 2 square
  double p[3][2] = {{0.0, 0.0},
                    {4.0, 3.5},
                    {2.5, 0.0}};
  struct polygon* o = polygon_new(4, p, color);
  double A_act = 10.0 * 2.0;
  double A = polygon_area(o);
  ck_assert_double_eq_tol(A, A_act, 1e-6);
  for(double rad = 0.0; rad < 2*M_PI; rad += (2*M_PI)*0.1) {
    polygon_rotate_rad(o, rad);
    for(unsigned i = 0; i < 10; i++) {
      double shift[2] = { random_double(-1.0, 1.0), random_double(-1.0, 1.0) };
      polygon_translate(o, shift, 1.0);
      A = polygon_area(o);
      ck_assert_double_eq_tol(A, A_act, 1e-6);
    } 
  }
}
END_TEST

Suite* matrix_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Polygon");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_triangle_area);
    tcase_add_test(tc_core, test_polygon_area);
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

