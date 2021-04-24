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
  // 10 * 2 square
  double p[4][2] = {{0.0, 2.0},
                    {10.0, 2.0},
                    {10.0, 0.0},
                    {0.0, 0.0}};
  struct polygon* o = polygon_new(4, p);
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
  // 10 * 2 square
  double p[3][2] = {{0.0, 0.0},
                    {4.0, 3.5},
                    {2.5, 0.0}};
  struct polygon* o = polygon_new(4, p);
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

START_TEST(test_generate) {
  for(unsigned i = 0; i < 25; i++) {
    unsigned nVertices = random_int(5, 20);
    double r = random_double(1.0, 100.0);
    struct polygon* poly = polygon_generate(nVertices, r);
    ck_assert_int_eq(nVertices, polygon_nVertices(poly));
  }
} END_TEST

START_TEST(test_create_sub) {
  for(unsigned i = 0; i < 5; i++) {
    unsigned nVertices = 5;
    double r = random_double(1.0, 100.0);
    struct polygon* poly = polygon_generate(nVertices, r);
    unsigned subVertices1[3] = {0, 1, 2};
    struct polygon* subPoly1 = polygon_create_sub(poly, 3, subVertices1);
    unsigned subVertices2[3] = {1, 2, 3};
    struct polygon* subPoly2 = polygon_create_sub(poly, 3, subVertices2);
    unsigned subVertices3[3] = {2, 3, 4};
    struct polygon* subPoly3 = polygon_create_sub(poly, 3, subVertices3);
    unsigned subVertices4[3] = {3, 4, 0};
    struct polygon* subPoly4 = polygon_create_sub(poly, 3, subVertices4);
    unsigned subVertices5[3] = {4, 0, 1};
    struct polygon* subPoly5 = polygon_create_sub(poly, 3, subVertices5);
  }
} END_TEST


START_TEST(test_edge_angle) {
  struct polygon* square = new_square(50, 50, 19);
  ck_assert_double_eq_tol(polygon_edge_angle(square, 0, 1), 0.5 * M_PI, 1e-4); 
  ck_assert_double_eq_tol(polygon_edge_angle(square, 1, 2), 0.5 * M_PI, 1e-4); 
  ck_assert_double_eq_tol(polygon_edge_angle(square, 2, 3), 0.5 * M_PI, 1e-4); 
  ck_assert_double_eq_tol(polygon_edge_angle(square, 3, 0), 0.5 * M_PI, 1e-4); 
} END_TEST

START_TEST(test_copy) {
  struct polygon* o = polygon_generate(20, 5.0);
  struct polygon* copy = polygon_copy(o);
  ck_assert_int_eq(polygon_nVertices(o), polygon_nVertices(copy));
  ck_assert_int_eq(polygon_nVertices(o), polygon_nVertices(copy));
  polygon_delete(o);
  polygon_delete(copy);
  ck_assert(matrix_eq(&o->vertices, &copy->vertices));
  ck_assert(matrix_eq(&o->edge_normals, &copy->edge_normals));
  ck_assert(matrix_eq(&o->edge_midpoints, &copy->edge_midpoints));
} END_TEST

Suite* polygon_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Polygon");
    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_generate);
    tcase_add_test(tc_core, test_edge_angle);
    tcase_add_test(tc_core, test_copy);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = polygon_suite();
    sr = srunner_create(s);

#ifdef FORK
    enum fork_status fork_status = CK_FORK_GETENV;
    if(FORK == 0) {
      fork_status = CK_NOFORK;
    } else if(FORK == 1) {
      fork_status = CK_FORK;
    }
    srunner_set_fork_status(sr, fork_status);
#endif

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

