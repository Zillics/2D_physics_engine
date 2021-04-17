#include "color.h"
#include "utils.h"
#include <stdio.h>

struct color color_black() {
  struct color black = BLACK;
  return black;
}

struct color color_red() {
  struct color red = RED;
  return red;
}

struct color color_green() {
  struct color green = GREEN;
  return green;
}

struct color color_blue() {
  struct color blue = BLUE; 
  return blue;
}

struct color color_white() {
  struct color white = WHITE; 
  return white;
}

struct color color_gray() {
  struct color gray = GRAY;
  return gray;
}

struct color color_random() {
  struct color random = { .r=random_int(0, 255), .g=random_int(0, 255), .b=random_int(0, 255) };
  return random;
}

void color_render(struct color* clr, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, clr->r, clr->g, clr->b, SDL_ALPHA_OPAQUE);
}

void color_print(struct color* clr) {
  printf("Color: %d %d %d\n", clr->r, clr->g, clr->b);
}
