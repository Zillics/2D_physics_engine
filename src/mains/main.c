#include <SDL2/SDL.h>
#include "../polygon.h"
#include "../polygon_container.h"
#include "../point.h"

uint32_t dt_ms = 1;

int main(int argc, char* argv[])
{
  struct polygon_container* oc = new_polygon_container();
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

                while (SDL_PollEvent(&event)) {
                    switch(event.type) {
                      case SDL_MOUSEBUTTONDOWN:
                        if(event.button.button == SDL_BUTTON_LEFT) {
                          insert_square(oc, event.button.x, event.button.y, 30);
                        }
                        if(event.button.button == SDL_BUTTON_RIGHT) {
                          double p[2] = {event.button.x, event.button.y};
                          for(unsigned i = 0; i < oc->nObjects; i++) {
                            if(polygon_contains(oc->polygons + i, p)) {
                              polygon_rotate(oc->polygons + i, 10);
                              //polygon_translate(oc->polygons + i, 0, 5);
                            } else {
                            }
                          }
                        }
                        break;
                      case SDL_MOUSEBUTTONUP:
                        break;
                      case SDL_QUIT:
                        done = SDL_TRUE;
                        break;
                    }
                }
                SDL_Delay(dt_ms);
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
