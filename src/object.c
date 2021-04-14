#include "object.h"

struct object* object_new(struct polygon shape) {
  struct object* o = malloc(sizeof(struct object));
  o->shape = shape;
  double direction[3] = { 0.0, 1.0, 1.0 };
  o->direction = *vector_create(direction, 3);
  o->state = *vector_new(9, 0.0);
  object_reset_state(o);
  o->density = 1.0;
  object_triangulate(o);
  return o;
}

struct object* object_generate(unsigned nPoints, double r) {
  struct polygon shape = *polygon_generate(nPoints, r);
  return object_new(shape);
}

struct object* new_square_object(double width, double density) {
  struct polygon shape = *new_square(0.0, 0.0, width);
  return object_new(shape);
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
       + polygon_container_size(&o->triangles)
       + matrix_size(&o->state)
       + matrix_size(&o->direction);
}

void object_triangulate(struct object* o) {
  unsigned nTriangles = polygon_nVertices(&o->shape) - 3;
  unsigned nEars;
  polygon_container_reset(&o->triangles, nTriangles, 3);
  ear_clipping(&o->shape, o->triangles.polygons, &nEars);
  assert(nEars == nTriangles);
}

bool objects_collide(struct object* o1, struct object* o2) {
  for(unsigned i = 0; i < o1->triangles.nPolygons; i++) {
    for(unsigned j = 0; j < o2->triangles.nPolygons; j++) {
      if(polygons_collide(o1->triangles.polygons + i, o2->triangles.polygons + j)) {
        return true;
      }
    }
  }
  return false;
}

void object_render(struct object* o, SDL_Renderer* renderer){
  polygon_render(&o->shape, renderer); 
}

void object_render_all(struct object* o, SDL_Renderer* renderer) {
  struct color clr;
  if(o->collides) {
    clr = color_red();
  } else {
    clr = color_green();
  }
  SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, SDL_ALPHA_OPAQUE);
  polygon_render(&o->shape, renderer);
  struct color gray = color_gray();
  SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, SDL_ALPHA_OPAQUE);
  for(unsigned i = 0; i < o->triangles.nPolygons; i++) {
    polygon_render(o->triangles.polygons + i, renderer);
  }
}

void object_tick(struct object* o, double dt) {
  double dx = object_vel(o)[0] * dt + object_acc(o)[0] * dt * dt;
  double dy = object_vel(o)[1] * dt + object_acc(o)[1] * dt * dt;
  double ddx = object_acc(o)[0] * dt;
  double ddy = object_acc(o)[1] * dt;
  double dp[2] = { dx, dy };
  object_translate(o, dp, 1.0);
  double da = object_angle(o)[0] + object_angvel(o)[0] * dt + object_angacc(o)[0] * dt * dt;
  object_rotate(o, da);
  // Update state
  *(object_pos(o) + 0) += dx;
  *(object_pos(o) + 1) += dy;
  *(object_vel(o) + 0) += ddx;
  *(object_vel(o) + 1) += ddy;
}

void object_place(struct object* o, double x, double y) {
  double target_[3] = { x, y, 1.0 };
  struct matrix* target = vector_create(target_, 3);
  struct matrix move = matrix_subtract(target, &o->shape.centroid);
  object_translate(o, move.data, 1.0);
}

void object_translate(struct object* o, double* v, double k) {
  polygon_translate(&o->shape, v, k);
  for(unsigned i = 0; i < o->triangles.nPolygons; i++) {
    polygon_translate(o->triangles.polygons + i, v, k); 
  }
}

void object_rotate(struct object* o, double deg) {
  polygon_rotate_deg(&o->shape, deg);
  matrix_rotate_deg(&o->direction, deg);
  for(unsigned i = 0; i < o->triangles.nPolygons; i++) {
    polygon_rotate_deg(o->triangles.polygons + i, deg);
  }
}

double* object_pos(struct object* o) { return o->state.data + 0; }
double* object_vel(struct object* o) { return o->state.data + 2; }
double* object_acc(struct object* o) { return o->state.data + 4; }
double* object_angle(struct object* o) { return o->state.data + 6; }
double* object_angvel(struct object* o) { return o->state.data + 7; }
double* object_angacc(struct object* o) { return o->state.data + 8; }

void object_print(struct object* o) {
  printf("OBJECT\n");
  printf("shape:\n");
  polygon_print(&o->shape);
  printf("state:\n");
  matrix_print(&o->state);
}
