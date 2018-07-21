#include <Adafruit_NeoPixel.h> 
#include <RF24.h>
#include "displays.h"
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_NeoPixel pixels;
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

uint8_t DRAW_OFFSET = 150;

unsigned long lastDrawTime = 0;
bool needsUpdate = true;

byte id;
bool isMaster;

// For now just place whatever test pattern you want to use here. 
void (*mainUpdate) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) = displays[1].updateFunc;
void (*mainInit) (struct Pixel newGrid[10][20]) = displays[1].initFunc;

void setup() {
  Serial.begin(9600);
  randomSeed(1);

  //Set the pin as an input
  pinMode(A0, INPUT);
  //Turn on the internal pullup
  digitalWrite(A0, HIGH);

  //Turn on the accelerometer
  if (! lis.begin(0x19)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Accelerometer couldnt start");
  }
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  
  lastDrawTime = millis();
  pixels = Adafruit_NeoPixel(50, 6, NEO_GRB);

  //Initialize the pixels
  pixels.begin();

  //Set the brightness to 16% (1/6th total brightness)
  //Param 1: Brightness from 0 - 255
  pixels.setBrightness(10);

  mainInit(newGrid);
  memcpy(oldGrid,newGrid,sizeof(newGrid));

  // Turn on the pixels, clearing whatever data was on them.
  pixels.show();
}

void loop() {
  //Get accelerometer data
  lis.read();

  // If we haven't calculated our new grid for this draw, do it.
  if (needsUpdate) {
    mainUpdate(oldGrid, newGrid);
    memcpy(oldGrid,newGrid,sizeof(newGrid));
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
