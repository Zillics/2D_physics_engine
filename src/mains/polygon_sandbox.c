#include <SDL2/SDL.h>
#include "../polygon.h"
#include "../object.h"
#include "../arena.h"
#include "../polygon_container.h"
#include "../polygon_algorithms.h"
#include "../linear_algebra.h"
#include "../utils.h"

#define FPS 100
#define DT_MS(X) 1000 / FPS
#define ROT_INCR 2
#define TRANS_INCR 2
#define PX 1600
#define PY 600
#define N_MAX_VERTICES 100

struct color default_color = GREEN;
struct color selected_color = WHITE;
struct color collision_color = RED;



int main(int argc, char* argv[])
{
  struct color color = GREEN;
  struct polygon* selected[2] = { NULL, NULL };
  unsigned nSelected = 0;
  unsigned iPrevSelected = 0;
  struct polygon_container* pc = polygon_container_new(0, 0);
  struct polygon_container* ears = polygon_container_new(0, 0);
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
              if(drawing) {
                for(unsigned i = 0; i < nVertices; i++) {
                  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                  SDL_RenderDrawPoint(renderer, vertices[i][0], vertices[i][1]);
                }
              }
              if(drawPolygons) {
                for(unsigned i = 0; i < pc->nPolygons; i++) {
                  if((pc->polygons + i) == selected[0] || (pc->polygons + i) == selected[1]) {
                    color_render(&selected_color, renderer);
                  } else {
                    color_render(&default_color, renderer);
                  }
                  polygon_render(pc->polygons + i, renderer);
                }
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
                          if(!drawing) {
                            polygon_container_reset(ears, 0, 0);
                            drawing = true;
                          }
                          vertices[nVertices][0] = event.button.x;
                          vertices[nVertices][1] = event.button.y;
                          nVertices += 1;
                        }
                          break;
                        case SDL_BUTTON_RIGHT:
                        {
                          if(drawing) {
                            // DRAW MODE
                            if(nVertices >= 3) {
                              struct polygon* poly = polygon_new(nVertices, vertices);
                              polygon_container_append(pc, poly);
                            }
                            nVertices = 0;
                            drawing = false;
                          } else {
                            // SELECT MODE
                            double x = event.button.x;
                            double y = event.button.y;
                            double p[2] = { x, y };
                            for(unsigned i = 0; i < pc->nPolygons; i++) {
                              struct polygon* poly = pc->polygons + i;
                              if(polygon_contains(poly, p)) {
                                // SELECT
                                if(poly == selected[0]) {
                                  selected[0] = selected[1];
                                  selected[1] = NULL;
                                  break;
                                }
                                if(poly == selected[1]) {
                                  selected[1] = NULL;
                                  break;
                                }
                                // DESLECT
                                nSelected = imax(nSelected + 1, 2);
                                selected[iPrevSelected] = poly;
                                iPrevSelected = (iPrevSelected + 1) % 2;
                                break;
                              }
                            }
                          }
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
                            if(nSelected > 0) {
                              struct polygon* poly = selected[nSelected - 1];
                              printf("Performing ear clipping\n");
                              // Ear clipping
                              polygon_container_reset(ears, polygon_nVertices(poly), 3);
                              unsigned nEars = 0;
                              ear_clipping(poly, ears->polygons, &nEars);
                              printf("Resizing ears...\n");
                              polygon_container_resize(ears, nEars);
                              drawPolygons = true;
                              printf("Ear clipping performed. Ears: %d, N: %d\n", nEars, polygon_nVertices(poly));
                            }
                          }
                          break;
                        case SDL_SCANCODE_S:
                          {
                            printf("Removing vertex\n");
                            unsigned idx = random_int(0, polygon_nVertices(selected[iPrevSelected]));
                            polygon_remove_vertex(selected[iPrevSelected], 2);
                          }
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
