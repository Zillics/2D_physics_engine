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
struct color color_white();
void color_print(struct color* clr);

#define RED { .r=255, .g=0, .b=0 }
#define GREEN { .r=0, .g=255, .b=0 }
#define BLUE { .r=0, .g=0, .b=255 }
#define BLACK { .r=0, .g=0, .b=0 }
#define WHITE { .r=255, .g=255, .b=255 }

#endif
