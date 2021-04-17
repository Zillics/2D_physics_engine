#include "../polygon_machine.h"

int main(int argc, char* argv[])
{
  struct polygon_machine* pm = polygon_machine_new();
  pm->x_px = 1600;
  pm->y_px = 1000;
  double dt = 0.01;
  polygon_machine_run(pm, 0.01);
  polygon_machine_delete(pm);
  return 0;
}
