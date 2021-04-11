#include "color.h"

struct color color_black() {
  struct color black = { .r=0, .g=0, .b=0 };
  return black;
}

struct color color_red() {
  struct color red = { .r=255, .g=0, .b=0 };
  return red;
}

struct color color_green() {
  struct color green = { .r=0, .g=255, .b=0 };
  return green;
}

struct color color_blue() {
  struct color blue = { .r=0, .g=0, .b=255 };
  return blue;
}
