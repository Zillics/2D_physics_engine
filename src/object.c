#include "object.h"
  
void object_delete(struct object* o){
  polygon_delete(&o->shape);
  matrix_delete(&o->vel);
  matrix_delete(&o->acc);
  matrix_delete(&o->angular_vel);
  matrix_delete(&o->angular_acc);
}

size_t object_size(struct object* o){
  return sizeof(double)
       + polygon_size(&o->shape)
       + matrix_size(&o->vel)
       + matrix_size(&o->acc)
       + matrix_size(&o->angular_vel)
       + matrix_size(&o->angular_acc);
}

void object_render(struct object* o, SDL_Renderer* renderer){
  polygon_render(&o->shape, renderer); 
}

struct object* new_square_object(double x, double y, double width, struct color color, double mass) {
  struct object* o = malloc(sizeof(struct object));
  o->shape = *new_square(x, y, width, color);
  o->mass = mass;
  object_init(o);
  return o;
}

void object_init(struct object* o) {
  o->vel = *matrix_new(3, 1, 0.0);
  *matrix_element(&o->vel, 2, 0) = 1.0;
  o->acc = *matrix_new(3, 1, 0.0);
  *matrix_element(&o->acc, 2, 0) = 1.0;
  o->angular_vel = *matrix_new(3, 1, 0.0);
  *matrix_element(&o->angular_vel, 2, 0) = 1.0;
  o->angular_acc = *matrix_new(3, 1, 0.0);
  *matrix_element(&o->angular_acc, 2, 0) = 1.0;  
}

