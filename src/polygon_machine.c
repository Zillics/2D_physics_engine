#include "polygon_machine.h"
#include "polygon_algorithms.h"
#include "utils.h"


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
  self->marker_color = color_red();
  self->vector_color = color_blue();
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
  self->mode = DRAW;
  self->n_draw_vertices = 0;
  self->n_draw_vector_points = 0;
  self->vector_drawn = false;
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

  if(!self->vector_drawn) {
    for(unsigned i = 0; i < self->n_draw_vector_points; i++) {
      color_render(&self->default_color, self->renderer);
      SDL_RenderDrawPoint(self->renderer, self->draw_vector[i][0], self->draw_vector[i][1]);
    }
  }
  if(self->vector_drawn && self->n_draw_vector_points == 2) {
    color_render(&self->vector_color, self->renderer);
    SDL_RenderDrawLine(self->renderer, self->draw_vector[0][0], self->draw_vector[0][1], self->draw_vector[1][0], self->draw_vector[1][1]);
  }

  // Render all markers
  for(unsigned i = 0; i < self->n_markers; i++) {
    color_render(&self->marker_color, self->renderer);
    SDL_RenderDrawPoint(self->renderer, self->markers[i][0], self->markers[i][1]);
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
    case DRAW_VECTOR:
    {
      self->iVectorpoint = (self->iVectorpoint + 1) % 2;
      self->draw_vector[self->iVectorpoint][0] = event->button.x;
      self->draw_vector[self->iVectorpoint][1] = event->button.y;
      self->n_draw_vector_points = imax(self->n_draw_vector_points + 1, 2);
      self->vector_drawn = self->n_draw_vector_points == 2;
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
    case DRAW_VECTOR:
    {
      break;
    }
  }
}

void polygon_machine_handle_key_down(struct polygon_machine* self, SDL_Event* event) {
   switch(event->key.keysym.scancode) {
    case SDL_SCANCODE_1:
    {
      self->mode = DRAW;
      break;
    }
    case SDL_SCANCODE_2:
    {
      self->mode = GENERATE;
      break;
    }
    case SDL_SCANCODE_3:
    {
      self->mode = SELECT;
      break;
    }
    case SDL_SCANCODE_4:
    {
      self->mode = DRAW_VECTOR;
      self->n_draw_vector_points = 0;
      self->vector_drawn = false;
      break;
    }
    case SDL_SCANCODE_ESCAPE:
    {
      polygon_machine_reset(self);
      break;
    }
    case SDL_SCANCODE_Q:
    {
      if(self->selected[0] >= 0) {
        polygon_print(self->polygons.polygons + self->selected[0]);
      }
      if(self->selected[1] >= 0) {
        polygon_print(self->polygons.polygons + self->selected[1]);
      }
      break;
    }
    case SDL_SCANCODE_W:
    {
      struct polygon* poly1 = polygon_machine_selected(self, 0);
      struct polygon* poly2 = polygon_machine_selected(self, 1);
      if(poly1 != NULL && poly2 != NULL) {
        bool collide = polygons_collide(poly1, poly2);
        if(collide) {
          printf("POLYGONS COLLIDE!\n");
        } else {
          printf("POLYGONS DO NOT COLLIDE!\n");
        }
      }
      break;
    }
    case SDL_SCANCODE_E:
    {
      struct polygon* poly = polygon_machine_selected(self, 0);
      if(poly != NULL) {
        self->n_markers = 0;
        struct matrix dir = vector_subtract_raw(self->draw_vector[0], self->draw_vector[1], 2);
        struct matrix p = furthest_point(poly, &dir);
        polygon_machine_add_marker(self, vector_value(&p, 0), vector_value(&p, 1));
      }
      break;
    }
    case SDL_SCANCODE_R:
    {
      struct polygon* poly1 = polygon_machine_selected(self, 0);
      struct polygon* poly2 = polygon_machine_selected(self, 1);
      struct matrix* penetration = vector_new(2, 0.0);
      if(poly1 != NULL && poly2 != NULL) {
        bool collide = polygons_collide_penetration(poly1, poly2, penetration);
        if(collide) {
          printf("POLYGONS COLLIDE!\n");
          polygon_translate(poly1, penetration->data, -1.0);
          matrix_print(penetration);
        } else {
          printf("POLYGONS DO NOT COLLIDE!\n");
        }
      }
      break;
    }
    case SDL_SCANCODE_T:
    {
      struct polygon* poly1 = polygon_machine_selected(self, 0);
      if(poly1 != NULL) {
        if(self->n_draw_vertices > 0) {
          polygon_insert_vertex(poly1, self->draw_vertices[self->n_draw_vertices - 1], polygon_nVertices(poly1));
        }
      }
      break;
    }
    default:
      break;
  }
}

struct polygon* polygon_machine_selected(struct polygon_machine* self, int i) {
  if(self->selected[i] >= 0 && self->polygons.nPolygons > i) {
    return self->polygons.polygons + self->selected[i];
  }
  return NULL;
}

bool polygon_machine_is_selected(struct polygon_machine* self, int i) {
  return self->selected[0] == i || self->selected[1] == i;
}

void polygon_machine_add_marker(struct polygon_machine* self, double x, double y) {
  if(self->n_markers > MAX_MARKERS) {
    return;
  }
  self->markers[self->n_markers][0] = x;
  self->markers[self->n_markers][1] = y;
  self->n_markers += 1;
}
