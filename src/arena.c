#include "arena.h"
#include "color.h"


struct arena* arena_new(double x_min, double y_min, double x_max, double y_max){
  struct arena* a = malloc(sizeof(struct arena));
  a->objects = *object_container_new();
  double width = x_max - x_min;
  double height = y_max - y_min;
  struct color color = RED;
  a->border = *new_square(x_min + 0.5*width, y_min + 0.5*height, width, color);
  return a;
}
void arena_delete(struct arena* a) {
  object_container_delete(&a->objects);
  polygon_delete(&a->border);
}

void arena_tick(struct arena* a, double dt) {
  struct object* o;
  unsigned N = a->objects.nObjects;
  for(unsigned i = 0; N; i++) {
    o = a->objects.objects + i;
    object_tick(o, dt);
    o->collides = false;
    o->shape.color = color_green();
  }
  for(unsigned i = 0; i < N; i++) {
    o = a->objects.objects + i;
    for(unsigned j = 0; j < N - 1; j++) {
      struct object* o2 = a->objects.objects + ((i + 1 + j) % N);
      if(objects_collide(o, o2)) {
        o->collides = true;
        o2->collides = true;
        o->shape.color = color_red();
      }
    }
  }
  a->t += dt;
}

void arena_render(struct arena* a, SDL_Renderer* renderer) {
  for(unsigned i = 0; i < a->objects.nObjects; i++) {
    object_render(a->objects.objects + i, renderer);
  }
  polygon_render(&a->border, renderer);
}

void arena_generate_object(struct arena* a, struct state* min_state, struct state* max_state) {

}
