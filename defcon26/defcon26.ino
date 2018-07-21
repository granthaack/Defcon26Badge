#include <Adafruit_NeoPixel.h> 
#include <RF24.h>

Adafruit_NeoPixel pixels;

uint8_t DRAW_OFFSET = 15;

unsigned long lastDrawTime = 0;
bool needsUpdate = true;

struct Pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Pixel oldGrid[10][20];
struct Pixel newGrid[10][20];

byte id;
bool isMaster;

void clearInit(struct Pixel newGrid[10][20]) {
  memset(newGrid,0,sizeof(newGrid));
}

void updateGrid(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) {
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

// For now just place whatever test pattern you want to use here. 
// Eventually there will be an array of function pointers which we will use to change these via buttons. 
void (*mainUpdate) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) = &updateGrid;
void (*mainInit) (struct Pixel newGrid[10][20]) = &clearInit;

void setup() {
  randomSeed(1);
  lastDrawTime = millis();
  pixels = Adafruit_NeoPixel(50, 6, NEO_GRB);

  //Initialize the pixels
  pixels.begin();

  //Set the brightness to 16% (1/6th total brightness)
  //Param 1: Brightness from 0 - 255
  pixels.setBrightness(20);

  // Turn on the pixels, clearing whatever data was on them.
  pixels.show();
}

void loop() {
  // If we haven't calculated our new grid for this draw, do it.
  if (needsUpdate) {
    mainUpdate(oldGrid, newGrid);
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 10; j++) {
        Pixel p = newGrid[i][j];
        pixels.setPixelColor(j + i * 10, p.r, p.g, p.b);
      }
    }
    needsUpdate = false;
  }

  // Wait DRAW_OFFSET number of milliseconds between each draw, to prevent LED flicker.
  unsigned long curTime = millis();
  if (curTime > lastDrawTime + DRAW_OFFSET) {
    lastDrawTime = curTime;
    pixels.show();
    needsUpdate = true;
  }
}