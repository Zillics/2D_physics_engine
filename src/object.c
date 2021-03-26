#include "object.h"

struct object* object_generate(unsigned nPoints, double r) {
  struct object* o = malloc(sizeof(struct object));
  o->shape = *polygon_generate(nPoints, r);
  double direction[3] = { 0.0, 1.0, 1.0 };
  o->direction = *vector_create(direction, 3);
  o->state = *vector_new(9, 0.0);
  object_reset_state(o);
  o->mass = 1.0;
  return o;
}

struct object* new_square_object(double width, struct color color, double mass) {
  struct object* o = malloc(sizeof(struct object));
  o->shape = *new_square(0.0, 0.0, width, color);
  double direction[3] = { 0.0, 1.0, 1.0 };
  o->direction = *vector_create(direction, 3);
  o->state = *vector_new(9, 0.0);
  object_reset_state(o);
  o->mass = mass;
  return o;
}

void object_delete(struct object* o){
  polygon_delete(&o->shape);
  matrix_delete(&o->direction);
  matrix_delete(&o->state);
}

void object_reset_state(struct object* o) {
  matrix_reset(&o->state, 0.0);
  *vector_element(&o->state, 0) = *vector_element(&o->shape.centroid, 0);
  *vector_element(&o->state, 1) = *vector_element(&o->shape.centroid, 1);
}

size_t object_size(struct object* o){
  return sizeof(struct object)
       + polygon_size(&o->shape)
       + matrix_size(&o->state)
       + matrix_size(&o->direction);
}

bool objects_collide(struct object* o1, struct object* o2) {
  return polygons_collide(&o1->shape, &o2->shape);
}

void object_render(struct object* o, SDL_Renderer* renderer){
  polygon_render(&o->shape, renderer); 
}

void object_tick(struct object* o, double dt) {
  // TODO
}

void object_place(struct object* o, double x, double y) {
  double target_[3] = { x, y, 1.0 };
  struct matrix* target = vector_create(target_, 3);
  struct matrix move = matrix_subtract(target, &o->shape.centroid);
  polygon_translate(&o->shape, move.data, 1.0);
}

void object_translate(struct object* o, double* v, double k) {
  polygon_translate(&o->shape, v, k);
}

void object_rotate(struct object* o, double deg) {
  polygon_rotate_deg(&o->shape, deg);
  matrix_rotate_deg(&o->direction, deg);
}



