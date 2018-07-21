#include <Arduino.h>

struct Pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Pixel oldGrid[10][20];
struct Pixel newGrid[10][20];