#include "object.h"

struct state* state_new() {
  struct state* s = malloc(sizeof(struct state));
  s->pos = *matrix_new(3, 1, 0.0);
  s->vel = *matrix_new(3, 1, 0.0);
  s->acc = *matrix_new(3, 1, 0.0);
  s->angular_vel = *matrix_new(3, 1, 0.0);
  s->angular_acc = *matrix_new(3, 1, 0.0);
  *matrix_element(&s->pos, 2, 0) = 1.0;
  *matrix_element(&s->vel, 2, 0) = 1.0;
  *matrix_element(&s->acc, 2, 0) = 1.0;
  *matrix_element(&s->angular_vel, 2, 0) = 1.0;
  *matrix_element(&s->angular_acc, 2, 0) = 1.0;
  return s;
}

void state_reset(struct state* s, double* p, double* v, double* a, double* av, double* aa) {
  *matrix_element(&s->pos, 0, 0) = p[0];
  *matrix_element(&s->pos, 1, 0) = p[1];
  *matrix_element(&s->vel, 0, 0) = v[0];
  *matrix_element(&s->vel, 1, 0) = v[1];
  *matrix_element(&s->acc, 0, 0) = a[0];
  *matrix_element(&s->acc, 1, 0) = a[1];
  *matrix_element(&s->angular_vel, 0, 0) = av[0];
  *matrix_element(&s->angular_vel, 1, 0) = av[1];
  *matrix_element(&s->angular_acc, 0, 0) = aa[0];
  *matrix_element(&s->angular_acc, 1, 0) = aa[1];
}

void state_delete(struct state* s) {
  matrix_delete(&s->vel);
  matrix_delete(&s->acc);
  matrix_delete(&s->angular_vel);
  matrix_delete(&s->angular_acc);
}

size_t state_size(struct state* s) {
  return matrix_size(&s->pos)
       + matrix_size(&s->vel)
       + matrix_size(&s->acc)
       + matrix_size(&s->angular_vel)
       + matrix_size(&s->angular_acc);
}

void object_delete(struct object* o){
  polygon_delete(&o->shape);
  state_delete(&o->state);
}

size_t object_size(struct object* o){
  return sizeof(double)
       + polygon_size(&o->shape)
       + state_size(&o->state);
}

bool objects_collide(struct object* o1, struct object* o2) {
  return polygons_collide(&o1->shape, &o2->shape);
}

void object_render(struct object* o, SDL_Renderer* renderer){
  polygon_render(&o->shape, renderer); 
}

void object_tick(struct object* o, double dt) {

}

void object_translate(struct object* o, double* v, double k) {
  polygon_translate(&o->shape, v, k);
}

void object_rotate(struct object* o, double deg) {
  polygon_rotate_deg(&o->shape, deg);
  matrix_rotate_deg(&o->direction, deg);
}

struct object* new_square_object(double width, struct color color, double mass) {
  struct object* o = malloc(sizeof(struct object));
  o->shape = *new_square(0.0, 0.0, width, color);
  double direction[3] = { 0.0, 1.0, 1.0 };
  o->direction = *vector_create(direction, 3);
  o->mass = mass;
  return o;
}

void object_reset_state(struct object* o, double* p, double* v, double* a, double* av, double* aa) {
  state_reset(&o->state, p, v, a, av, aa);
}



