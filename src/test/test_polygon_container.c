#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "../polygon.h"
#include "../polygon_container.h"
#include "../linear_algebra.h"
#include "../utils.h"

START_TEST(test_smoke) {
  struct polygon_container* pc1 = polygon_container_new(2, 5);
  polygon_container_reset(pc1, 20, 10);
  polygon_container_reset(pc1, 120, 11);
  polygon_container_reset(pc1, 1, 3);
  struct polygon_container* pc2 = polygon_container_new(2, 10);
  polygon_container_reset(pc2, 20, 10);
  polygon_container_reset(pc2, 20, 11);
  polygon_container_reset(pc2, 20, 12);
  polygon_container_delete(pc1);
  polygon_container_delete(pc2);
}

START_TEST(test_append) {
  struct polygon_container* pc = polygon_container_new(0, 0);
  double r = 4.0;
  ck_assert_int_eq(0, pc->nPolygons); 
  for(unsigned i = 0; i < 50; i++) {
    struct polygon* poly = polygon_generate(random_int(3, 20), r);
    polygon_container_append(pc, poly);
  }
  ck_assert_int_eq(50, pc->nPolygons);
}
END_TEST

START_TEST(test_reset) {
  struct polygon_container* pc = polygon_container_new(0, 0);
  polygon_container_reset(pc, 25, 3);
  ck_assert_int_eq(25, pc->nPolygons);
  double r = 4.0;
  for(unsigned i = 0; i < 10; i++) {
    pc->polygons[i] = *polygon_generate(3, r);
  }
  ck_assert_int_eq(25, pc->nPolygons);
}

START_TEST(test_pop) {
  struct polygon_container* pc = polygon_container_new(0, 0);
  double r = 4.0;
  ck_assert_int_eq(0, pc->nPolygons); 
  for(unsigned i = 0; i < 50; i++) {
    struct polygon* poly = polygon_generate(random_int(3, 20), r);
    polygon_container_append(pc, poly);
  }
  ck_assert_int_eq(50, pc->nPolygons);
  for(unsigned i = 0; i < 50; i++) {
    polygon_container_pop(pc);
    ck_assert_int_eq(49 - i, pc->nPolygons);
  }
}
END_TEST

Suite* polygon_container_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("PolygonContainer");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_smoke);
    tcase_add_test(tc_core, test_append);
    tcase_add_test(tc_core, test_pop);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = polygon_container_suite();
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

