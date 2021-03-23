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
  for(unsigned i = 0; a->objects.nObjects; i++) {
    o = a->objects.objects + i;
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
