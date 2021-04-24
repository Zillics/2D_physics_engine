#include "polygon_machine.h"


struct polygon_machine* polygon_machine_new() {
  struct polygon_machine* self = malloc(sizeof(struct polygon_machine));
  self->window = NULL;
  self->renderer = NULL;
  self->x_px = 0;
  self->y_px = 0;
  self->polygons = *polygon_container_new(0, 0);
  self->background_color = color_black();
  self->default_color = color_green();
  self->selected_color = color_white();
  polygon_machine_reset(self);
  return self;
}

void polygon_machine_delete(struct polygon_machine* self) {
  polygon_container_delete(&self->polygons);
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
  self->stop = false;
  self->selected[0] = -1;
  self->selected[1] = -1;
  polygon_container_resize(&self->polygons, 0);
  self->mode = GENERATE;
  self->n_draw_vertices = 0;
}

void polygon_machine_render(struct polygon_machine* self) {
  // Render background
  struct color background_color = color_black();
  color_render(&background_color, self->renderer);
  SDL_RenderClear(self->renderer);

  // Render all polygons
  for(unsigned i = 0; i < self->polygons.nPolygons; i++) {
    // Render color
    if(polygon_machine_is_selected(self, i)) {
      color_render(&self->selected_color, self->renderer);
    } else {
      color_render(&self->default_color, self->renderer);
    }
    // Render polygon
    polygon_render(self->polygons.polygons + i, self->renderer);
  }

  // Render all draw points
  for(unsigned i = 0; i < self->n_draw_vertices; i++) {
    color_render(&self->default_color, self->renderer);
    SDL_RenderDrawPoint(self->renderer, self->draw_vertices[i][0], self->draw_vertices[i][1]);
  }

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
            polygon_machine_handle_left_click(self, event);
            break;
          }
        case SDL_BUTTON_RIGHT:
        {
          polygon_machine_handle_right_click(self, event);
          break;
        }
      }
      break;
    }
    case SDL_KEYDOWN:
    {
      polygon_machine_handle_key_down(self, event);
      break;
    }
    case SDL_QUIT:
    {
      self->stop = true;
      return;
    }
  }
}

void polygon_machine_handle_left_click(struct polygon_machine* self, SDL_Event* event) {
  switch(self->mode) {
    case DRAW:
    {
      self->draw_vertices[self->n_draw_vertices][0] = event->button.x;
      self->draw_vertices[self->n_draw_vertices][1] = event->button.y;
      self->n_draw_vertices += 1;
      break;
    }
    case GENERATE:
    {
      struct polygon* poly = polygon_generate(7, 50);
      polygon_place(poly, event->button.x, event->button.y);
      polygon_container_append(&self->polygons, poly);
      break;
    }
    case SELECT:
    {
      double p[3] = { event->button.x, event->button.y, 1.0 };
      for(unsigned i = 0; i < self->polygons.nPolygons; i++) {
        if(polygon_contains(self->polygons.polygons + i, p)) {
          if(!polygon_machine_is_selected(self, i)) {
            self->selected[self->iSelected] = i;
            self->iSelected = (self->iSelected + 1) % 2;
          }
          break;
        }
      }
      break;
    }
  }
}

void polygon_machine_handle_right_click(struct polygon_machine* self, SDL_Event* event) {
  switch(self->mode) {
    case DRAW:
    {
      if(self->n_draw_vertices >= 3) {
        struct polygon* poly = polygon_new(self->n_draw_vertices, self->draw_vertices);
        polygon_container_append(&self->polygons, poly);
        self->n_draw_vertices = 0;
      } else {
        self->n_draw_vertices = 0;
      }
      break;
    }
    case GENERATE:
    {
      break;
    }
    case SELECT:
    {
      // Deselect
      double p[3] = { event->button.x, event->button.y, 1.0 };
      for(unsigned i = 0; i < self->polygons.nPolygons; i++) {
        if(polygon_contains(self->polygons.polygons + i, p)) {
          if(i == self->selected[0]) {
            self->selected[0] = -1;
            self->iSelected = 0;
          }
          if(i == self->selected[1]) {
            self->selected[1] = -1;
            self->iSelected = 1;
          }
        }
      }
      break;
    }
  }
}

void polygon_machine_handle_key_down(struct polygon_machine* self, SDL_Event* event) {
   switch(event->key.keysym.scancode) {
    case SDL_SCANCODE_1:
    {
      self->mode = DRAW;
      self->n_draw_vertices = 0;
      break;
    }
    case SDL_SCANCODE_2:
    {
      self->mode = GENERATE;
      self->n_draw_vertices = 0;
      break;
    }
    case SDL_SCANCODE_3:
    {
      self->mode = SELECT;
      self->n_draw_vertices = 0;
      break;
    }
    case SDL_SCANCODE_ESCAPE:
    {
      polygon_machine_reset(self);
      break;
    }
    default:
      break;
  }
}


bool polygon_machine_is_selected(struct polygon_machine* self, int i) {
  return self->selected[0] == i || self->selected[1] == i;
}
