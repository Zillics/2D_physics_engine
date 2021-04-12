#include "color.h"
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


void color_print(struct color* clr) {
  printf("Color: %d %d %d\n", clr->r, clr->g, clr->b);
}
