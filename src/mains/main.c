#include <SDL2/SDL.h>
#include "../polygon.h"
#include "../object_container.h"
#include "../linear_algebra.h"

#define FPS 30
#define DT_MS(X) 1000 / FPS
#define ROT_INCR 2
#define TRANS_INCR 2

struct color default_color = GREEN;
struct color collision_color = RED;

int main(int argc, char* argv[])
{
  struct polygon* poly = new_square(200, 200, 30, default_color);
  struct matrix normals[4];
  struct matrix midpoints[4];
  for(unsigned i = 0; i < 4; i++) {
    normals[i] = polygon_edge_normal(poly, i, false);
    midpoints[i] = polygon_edge_midpoint(poly, i);
  }
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                // Render background
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                polygon_render(poly, renderer);
                for(unsigned i = 0; i < 4; i++) {
                  struct matrix* n = normals + i;
                  struct matrix* m = midpoints + i;
                  SDL_RenderDrawLine(renderer, matrix_value(m, 0, 0), matrix_value(m, 1, 0), matrix_value(m, 0, 0) + matrix_value(n, 0, 0), matrix_value(m, 1, 0) + matrix_value(n, 1, 0));
                }
                // Display all rendered stuff
                SDL_RenderPresent(renderer);
                while (SDL_PollEvent(&event)) {
                    switch(event.type) {
                      case SDL_MOUSEBUTTONDOWN:
                        switch(event.button.button) {
                          case SDL_BUTTON_LEFT: 
                            break;
                          case SDL_BUTTON_RIGHT:
                            break;
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

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        polygon_delete(poly);
    }
    SDL_Quit();
    return 0;
}
