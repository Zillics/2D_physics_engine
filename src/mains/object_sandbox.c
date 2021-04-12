#include <SDL2/SDL.h>
#include "../polygon.h"
#include "../object.h"
#include "../arena.h"
#include "../object_container.h"
#include "../linear_algebra.h"

#define FPS 30
#define DT_MS(X) 1000 / FPS
#define ROT_INCR 2
#define TRANS_INCR 2
#define PX 1600
#define PY 1000

#define DEFAULT_COLOR WHITE 
#define SELECTED_COLOR GREEN
#define COLLISION_COLOR RED

struct color default_color = DEFAULT_COLOR;
struct color selected_color = SELECTED_COLOR;
struct color collision_color = COLLISION_COLOR;

void spawn_object(double x, double y, struct arena* a) {
  struct object *o = object_generate(6, 100.0);
  o->shape.color = default_color; 
  object_place(o, x, y);
  double maxVel = 0.0;
  double maxAngVel = 0.0;
  double min_[9] = { x, y, -maxVel, -maxVel, 0.0, 0.0, -maxAngVel, 0.0, 0.0};
  double max_[9] = { x, y, maxVel, maxVel, 0.0, 0.0, maxAngVel, 0.0, 0.0};
  struct matrix* min = vector_create(min_, 9);
  struct matrix* max = vector_create(max_, 9);
  struct matrix* state = vector_generate(min, max);
  //o->state = *state;
  object_container_append(&a->objects, o);
  matrix_delete(min);
  matrix_delete(max);
  matrix_delete(state);
}

int main(int argc, char* argv[])
{
  struct color color = GREEN;
  struct arena* a = arena_new(0, 0, PX, PY);
  struct object* selected = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(PX, PY, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;
                // Render background
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                arena_tick(a, DT_MS(FPS));
                arena_render(a, renderer);
                // Display all rendered stuff
                SDL_RenderPresent(renderer);
                while (SDL_PollEvent(&event)) {
                    switch(event.type) {
                      case SDL_MOUSEBUTTONDOWN:
                        switch(event.button.button) {
                          case SDL_BUTTON_LEFT:
                            {
                              struct object* tmp = arena_find_object_at(a, event.button.x, event.button.y);
                              if(tmp) {
                                if(selected) {
                                  selected->shape.color = default_color;
                                }
                                tmp->shape.color = selected_color;
                                selected = tmp;
                                tmp = NULL;
                              }
                            }
                            break;
                          case SDL_BUTTON_RIGHT:
                            {
                              spawn_object(event.button.x, event.button.y, a);
                              break;
                            }
                        }
                        break;
                      case SDL_MOUSEBUTTONUP:
                        break;
                      case SDL_KEYDOWN:
                        switch(event.key.keysym.scancode) {
                          case SDL_SCANCODE_A:
                            break;
                          case SDL_SCANCODE_S:
                            break;
                          case SDL_SCANCODE_D:
                            break;
                          case SDL_SCANCODE_W:
                            break;
                          default:
                            break;
                        }
                        break;
                      case SDL_QUIT:
                        done = SDL_TRUE;
                        break;
                      default:
                        break;
                    }
                }
                SDL_Delay(DT_MS(FPS));
            }
        }
        arena_delete(a);
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}
