#include <SDL2/SDL.h>
#include "../polygon.h"
#include "../object.h"
#include "../arena.h"
#include "../object_container.h"
#include "../linear_algebra.h"

#define FPS 100
#define DT_MS(X) 1000 / FPS
#define ROT_INCR 2
#define TRANS_INCR 2
#define PX 1600
#define PY 600
#define N_MAX_VERTICES 100
#define POLYGON 1
#define POLYGON_DRAW 1
#define POLYGON_CLOCKWISE 0
#define LINE 0
#define LINE_DRAW 0

struct color default_color = GREEN;
struct color collision_color = RED;

int main(int argc, char* argv[])
{
  struct color color = GREEN;
  struct polygon* o = NULL;
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
#if POLYGON
                if(o) {
                  polygon_render(o, renderer);
                }
                if(drawing) {
                  for(unsigned i = 0; i < nVertices; i++) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint(renderer, vertices[i][0], vertices[i][1]);
                  }
                }
#endif
#if LINE
                if(drawing) {
                  for(unsigned i = 0; i < nVertices; i++) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint(renderer, vertices[i][0], vertices[i][1]);  
                  }
                } else {
                  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                  SDL_RenderDrawLine(renderer, vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1]);
                  SDL_RenderDrawLine(renderer, vertices[2][0], vertices[2][1], vertices[3][0], vertices[3][1]);
                }
#endif
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
#if POLYGON
                            if(nVertices >= 3) {
#if POLYGON_CLOCKWISE
                              struct matrix* temp = matrix_create(2, 3, vertices);
                              bool ccw = vectors_counter_clockwise_2D_raw(matrix_col_raw(temp, 0), matrix_col_raw(temp, 1), matrix_col_raw(temp, 2));
                              if(ccw) {
                                printf("counter-clockwise\n");
                              } else {
                                printf("clockwise\n");
                              }
                              nVertices = 0;
                              drawing = false;
#endif
#if POLYGON_DRAW
                              if(o) {
                                polygon_delete(o);
                              }
                              struct polygon* temp = polygon_new(nVertices, vertices, color);
                              if(polygon_self_intersects(temp)) {
                                printf("Polygon self intersects -> not simple! Only simple polygons accepted\n");
                              } else {
                                o = polygon_new(nVertices, vertices, color);
                              }
#endif
                            }
                            nVertices = 0;
                            drawing = false;
#endif
#if LINE
                            if(nVertices >= 4) {
                              drawing = false;
                              nVertices = 0;
                              struct matrix* temp = matrix_create(2, 4, vertices);
                              if(lines_intersect_2D_raw(matrix_col_raw(temp, 0), matrix_col_raw(temp, 1), matrix_col_raw(temp, 2), matrix_col_raw(temp, 3))) {
                                printf("Lines intersect!\n");
                              } else {
                                printf("Lines do not intersect!\n");
                              }
                              matrix_delete(temp);
                            }
#if LINE_DRAW
#endif
#endif
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
