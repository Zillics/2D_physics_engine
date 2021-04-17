#ifndef POLYGON_MACHINE_H
#define POLYGON_MACHINE_H

#include "sdl_state_machine.h"
#include "color.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

struct polygon_machine {
  SDL_Window* window;
  SDL_Renderer* renderer;
  int x_px;
  int y_px;
  bool stop;
};

struct polygon_machine* polygon_machine_new();
void polygon_machine_delete(struct polygon_machine* self);
void polygon_machine_handle_event(struct polygon_machine* self, SDL_Event* event);
void polygon_machine_tick(struct polygon_machine* self);
void polygon_machine_reset(struct polygon_machine* self);
void polygon_machine_run(struct polygon_machine* self, double dt);
void polygon_machine_render(struct polygon_machine* self);

#endif
