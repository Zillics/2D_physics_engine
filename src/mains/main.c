#include <SDL2/SDL.h>
#include "../polygon.h"
#include "../object.h"
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
  //struct polygon* poly = new_square(200, 200, 30, default_color);
  struct color color = GREEN;
  // 10 * 2 square
  double p[3][2] = {{0.0, 0.0},
                    {40.0, 30.0},
                    {20.0, 0.0}};
  struct object* o = new_square_object(30, color, 1.0);
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

                object_render(o, renderer);
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
                            object_rotate(o, -0.5);
                            break;
                          case SDL_SCANCODE_S:
                            object_translate(o, o->direction.data, 1.0);
                            break;
                          case SDL_SCANCODE_D:
                            object_rotate(o, 0.5);
                            break;
                          case SDL_SCANCODE_W:
                            object_translate(o, o->direction.data, -1.0);
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
        object_delete(o);
    }
    SDL_Quit();
    return 0;
}
