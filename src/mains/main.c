#include <SDL2/SDL.h>
#include "../polygon.h"
#include "../polygon_container.h"
#include "../linear_algebra.h"

#define FPS 30
#define DT_MS(X) 1000 / FPS
#define ROT_INCR 2
#define TRANS_INCR 2

struct color default_color = GREEN;
struct color collision_color = RED;

int main(int argc, char* argv[])
{
  struct polygon_container* oc = new_polygon_container();
  insert_square(oc, 200, 200, 30, default_color);
  insert_square(oc, 400, 100, 30, default_color);
  struct polygon* player = oc->polygons + 0;
  struct matrix* direction = matrix_new(3, 1, 0.0);
  *matrix_element(direction, 0, 0) = 1.0;
  *matrix_element(direction, 1, 0) = 0.0;
  *matrix_element(direction, 2, 0) = 1.0;
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

                // Render polygons
                render_polygons(oc, renderer);

                // Display all rendered stuff
                SDL_RenderPresent(renderer);
                if(polygons_collide(oc->polygons + 0, oc->polygons + 1)) {
                  oc->polygons[0].color = default_color;
                } else {
                  oc->polygons[0].color = collision_color;
                }
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
                            polygon_rotate(player, -ROT_INCR);
                            matrix_rotate(direction, -ROT_INCR);
                            break;
                          case SDL_SCANCODE_S:
                            polygon_translate(player, direction->data);
                            break;
                          case SDL_SCANCODE_D:
                            polygon_rotate(player, ROT_INCR);
                            matrix_rotate(direction, ROT_INCR);
                            break;
                          case SDL_SCANCODE_W:
                            polygon_translate(player, direction->data);
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
        delete_polygon_container(oc);
    }
    SDL_Quit();
    return 0;
}
