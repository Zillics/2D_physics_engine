#include <SDL2/SDL.h>
#include "../polygon.h"
#include "../object.h"
#include "../arena.h"
#include "../polygon_container.h"
#include "../linear_algebra.h"

#define FPS 100
#define DT_MS(X) 1000 / FPS
#define ROT_INCR 2
#define TRANS_INCR 2
#define PX 1600
#define PY 600
#define N_MAX_VERTICES 100

struct color default_color = GREEN;
struct color collision_color = RED;

int main(int argc, char* argv[])
{
  struct color color = GREEN;
  struct polygon* o = NULL;
  struct polygon_container* pc = polygon_container_new(0, 0);
  bool drawPolygons = false;
  double vertices[N_MAX_VERTICES][2]; 
  unsigned nVertices = 0;
  bool drawing = false;
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
                if(o) {
                  polygon_render(o, renderer);
                }
                if(drawing) {
                  for(unsigned i = 0; i < nVertices; i++) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint(renderer, vertices[i][0], vertices[i][1]);
                  }
                }
                if(drawPolygons) {
                  polygon_container_render(pc, renderer);
                }
                // Display all rendered stuff
                SDL_RenderPresent(renderer);
                while (SDL_PollEvent(&event)) {
                    switch(event.type) {
                      case SDL_MOUSEBUTTONDOWN:
                        {
                        switch(event.button.button) {
                          case SDL_BUTTON_LEFT:
                          {
                            vertices[nVertices][0] = event.button.x;
                            vertices[nVertices][1] = event.button.y;
                            nVertices += 1;
                            drawing = true;
                          }
                            break;
                          case SDL_BUTTON_RIGHT:
                          {
                            if(nVertices >= 3) {
                              if(o) {
                                polygon_delete(o);
                              }
                              struct polygon* temp = polygon_new(nVertices, vertices, color);
                              if(polygon_self_intersects(temp)) { 
                                printf("Polygon self intersects -> not simple! Only simple polygons accepted\n");
                                o = NULL;
                              } else {
                                o = temp;
                              }
                            }
                            nVertices = 0;
                            drawing = false;
                            break;
                          }
                          break;
                        }
                        break;
                      }
                      case SDL_MOUSEBUTTONUP:
                        break;
                      case SDL_KEYDOWN:
                        switch(event.key.keysym.scancode) {
                          case SDL_SCANCODE_A:
                            {
                              // Ear clipping
                              polygon_container_reset(pc, polygon_nVertices(o), 3);
                              ear_clipping(o, &pc->polygons);
                              drawPolygons = true;
                            }
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
    }
    SDL_Quit();
    return 0;
}
