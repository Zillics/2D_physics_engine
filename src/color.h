#ifndef COLOR_H
#define COLOR_H

struct color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

struct color color_black();
struct color color_red();
struct color color_green();
struct color color_blue();

#define RED { .r = 255, .g = 0, .b = 0 }
#define GREEN { .r = 0, .g = 255, .b = 0 }
#define BLUE { .r = 0, .g = 0, .b = 255 }

#endif
