#include <Adafruit_NeoPixel.h>
#include "menu.h"

Adafruit_NeoPixel pixels;
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

uint8_t DRAW_OFFSET = 150;

unsigned long lastDrawTime = 0;
bool needsUpdate = true;

void setup() {
  Serial.begin(9600);
  randomSeed(1);

  //Set the pin as an input
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  //Turn on the internal pullup
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);

  lastDrawTime = millis();
  pixels = Adafruit_NeoPixel(50, 6, NEO_GRB);

  //Initialize the pixels
  pixels.begin();

  //Set the brightness to 16% (1/6th total brightness)
  //Param 1: Brightness from 0 - 255
  pixels.setBrightness(10);

  mainInit = &menuInit;
  mainUpdate = &menuUpdate;

  mainInit(newGrid);
  memcpy(oldGrid,newGrid,sizeof(newGrid));

  // Turn on the pixels, clearing whatever data was on them.
  pixels.show();
}

void loop() {
  if(!isMenu && !digitalRead(A0) && !digitalRead(A3)) {
    isMenu = true;
    menuInit(newGrid);
    mainUpdate = &menuUpdate;
  }

  // If we haven't calculated our new grid for this draw, do it.
  if (needsUpdate) {
    mainUpdate(oldGrid, newGrid);
    memcpy(oldGrid,newGrid,sizeof(newGrid));
    uint8_t counter = 0;
    for (uint8_t i = 0; i < 5; i++) {
      for (uint8_t j = 0; j < 10; j++) {
        Pixel p = newGrid[i][j];
        pixels.setPixelColor(counter++, p.r, p.g, p.b);
      }
    }
    needsUpdate = false;
    if(isMaster) {
      sendGrid(newGrid);
    }
  }

  // Wait DRAW_OFFSET number of milliseconds between each draw, to prevent LED flicker.
  unsigned long curTime = millis();
  if (curTime > lastDrawTime + DRAW_OFFSET) {
    lastDrawTime = curTime;
    pixels.show();
    needsUpdate = true;
  }
}
