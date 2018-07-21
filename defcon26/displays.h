#ifndef DISPLAYS_H_   /* Include guard */
#define DISPLAYS_H_
#include "pixel.h"

void sampleInit(struct Pixel newGrid[10][20]);
void sampleUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

struct Display {
  void (*updateFunc) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);
  void (*initFunc) (struct Pixel newGrid[10][20]);
};

// We have 1kb of ROM, so shove everything we can in there.
// This stores all the functions we can use.
PROGMEM const struct Display displays[] =
{
  {&sampleUpdate,&sampleInit}
};

void sampleInit(struct Pixel newGrid[10][20]) {
  memset(newGrid,0,sizeof(newGrid));
}

void sampleUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 10; j++) {
      newGrid[i][j].r = newGrid[i][j].g = newGrid[i][j].b = 0;
      unsigned long type = random(0, 3);
      switch (type) {
      case 0:
        newGrid[i][j].r = 255;
        break;
      case 1:
        newGrid[i][j].g = 255;
        break;
      case 2:
        newGrid[i][j].b = 255;
        break;
      }
    }
  }
}
#endif // DISPLAYS_H_