#include <Arduino.h>

PROGMEM const uint8_t letterSlices[] =
{
  0b11111000,
  0b10001000,   
  0b01110000,   
  0b10101000, 
  0b10100000,  
  0b01000000,    
  0b00100000,
  0b10011000,
  0b00010000,
  0b00000000,
  0b01001000
};

struct Pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Pixel oldGrid[10][20];
struct Pixel newGrid[10][20];
