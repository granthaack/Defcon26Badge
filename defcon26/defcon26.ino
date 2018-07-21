#include <Adafruit_NeoPixel.h> 
#include <RF24.h>
#include "displays.h"

Adafruit_NeoPixel pixels;

uint8_t DRAW_OFFSET = 150;

unsigned long lastDrawTime = 0;
bool needsUpdate = true;

byte id;
bool isMaster;

// For now just place whatever test pattern you want to use here. 
void (*mainUpdate) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) = displays[0].updateFunc;
void (*mainInit) (struct Pixel newGrid[10][20]) = displays[0].initFunc;

void setup() {
  randomSeed(1);
  lastDrawTime = millis();
  pixels = Adafruit_NeoPixel(50, 6, NEO_GRB);

  //Initialize the pixels
  pixels.begin();

  //Set the brightness to 16% (1/6th total brightness)
  //Param 1: Brightness from 0 - 255
  pixels.setBrightness(10);

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
