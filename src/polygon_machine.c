#include "polygon_machine.h"


struct polygon_machine* polygon_machine_new() {
  struct polygon_machine* self = malloc(sizeof(struct polygon_machine));
  self->window = NULL;
  self->renderer = NULL;
  self->x_px = 0;
  self->y_px = 0;
  return self;
}

void polygon_machine_delete(struct polygon_machine* self) {
  if(self->renderer) {
    SDL_DestroyRenderer(self->renderer);
  }
  if(self->window) {
    SDL_DestroyWindow(self->window);
  }
  SDL_Quit();
}

void polygon_machine_tick(struct polygon_machine* self) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    polygon_machine_handle_event(self, &event);
  }
  polygon_machine_render(self);
}

void polygon_machine_reset(struct polygon_machine* self) {

}

void polygon_machine_render(struct polygon_machine* self) {
  // Render background
  struct color background_color = color_black();
  color_render(&background_color, self->renderer);
  SDL_RenderClear(self->renderer);

  // Insert all rendering here...

  // Display all rendered stuff
  SDL_RenderPresent(self->renderer);
}

void polygon_machine_run(struct polygon_machine* self, double dt) {

  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    return;
  }

  if (SDL_CreateWindowAndRenderer(self->x_px, self->y_px, 0, &self->window, &self->renderer) != 0) {
    return;
  }


  while(!self->stop) {
    SDL_Delay(dt);
    polygon_machine_tick(self);
  }
}

void polygon_machine_handle_event(struct polygon_machine* self, SDL_Event* event) {
  switch(event->type) {
    case SDL_MOUSEBUTTONDOWN:
    {
      switch(event->button.button) {
        case SDL_BUTTON_LEFT:
        {
        break;
        }
        case SDL_BUTTON_RIGHT:
        {
        break;
        }
      }
      break;
    }
    case SDL_QUIT:
    {
      self->stop = true;
      return;
    }
  }
}
