#include <Arduino.h>

const uint8_t letterSlices[] =
{
  0b00011111,
  0b00010001,   
  0b00001110,   
  0b00010101, 
  0b00010100,  
  0b00001000,    
  0b00000100,
  0b00010011,
  0b00000010,
  0b00000000,
  0b00001001
};

struct Pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Pixel oldGrid[10][20];
struct Pixel newGrid[10][20];
