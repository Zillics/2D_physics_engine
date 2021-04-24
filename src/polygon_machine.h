#ifndef POLYGON_MACHINE_H
#define POLYGON_MACHINE_H

#include "color.h"
#include "polygon_container.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

#define MAX_VERTICES 100
#define MAX_MARKERS 100

enum mode { DRAW, GENERATE, SELECT, DRAW_VECTOR };

struct polygon_machine {
  SDL_Window* window;
  SDL_Renderer* renderer;
  int x_px;
  int y_px;
  bool stop;
  struct color background_color;
  struct color selected_color;
  struct color default_color;
  struct color marker_color;
  struct color vector_color;
  int selected[2];
  unsigned iSelected;
  struct polygon_container polygons;
  double draw_vertices[MAX_VERTICES][2];
  unsigned n_draw_vertices;
  double draw_vector[2][2];
  unsigned iVectorpoint;
  bool vector_drawn;
  unsigned n_draw_vector_points;
  double markers[MAX_MARKERS][2];
  unsigned n_markers;
  enum mode mode;
};

struct polygon_machine* polygon_machine_new();
void polygon_machine_delete(struct polygon_machine* self);
void polygon_machine_handle_event(struct polygon_machine* self, SDL_Event* event);
void polygon_machine_handle_left_click(struct polygon_machine* self, SDL_Event* event);
void polygon_machine_handle_right_click(struct polygon_machine* self, SDL_Event* event);
void polygon_machine_handle_key_down(struct polygon_machine* self, SDL_Event* event);
void polygon_machine_tick(struct polygon_machine* self);
void polygon_machine_reset(struct polygon_machine* self);
void polygon_machine_run(struct polygon_machine* self, double dt);
void polygon_machine_render(struct polygon_machine* self);
struct polygon* polygon_machine_selected(struct polygon_machine* self, int i);
bool polygon_machine_is_selected(struct polygon_machine* self, int i);
void polygon_machine_add_marker(struct polygon_machine* self, double x, double y);

#endif
