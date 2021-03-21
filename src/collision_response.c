#include "collision_response.h"

struct matrix collision_response_direction(struct matrix* v1, struct matrix* collision_vertix) {
  struct matrix* n = matrix_new(1, 3, 0.0);
  *matrix_element(n, 0, 0) = matrix_value(collision_vertix, 1, 0);
  *matrix_element(n, 0, 1) = matrix_value(collision_vertix, 0, 0);
  double angle = vector_angle(n, v1); 
  matrix_rotate_rad(n, angle); 
  struct matrix ret = unit_vector(n);
  return ret;
}
