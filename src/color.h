#ifndef COLOR_H
#define COLOR_H

struct color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

#define RED { .r = 255, .g = 0, .b = 0 }
#define GREEN { .r = 0, .g = 255, .b = 0 }
#define BLUE { .r = 0, .g = 0, .b = 255 }

#endif
