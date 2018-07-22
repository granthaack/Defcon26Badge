#include <Simpix.h>
#include <Accel.h>
#include <RF24.h>
#include "displays.h"

uint8_t DRAW_OFFSET = 150;

unsigned long lastDrawTime = 0;
bool needsUpdate = true;

byte id;
bool isMaster;

// For now just place whatever test pattern you want to use here. 
void (*mainUpdate) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) = displays[1].updateFunc;
void (*mainInit) (struct Pixel newGrid[10][20]) = displays[1].initFunc;

Accel accel;

void setup() {
  Serial.begin(9600);
  init_accel();
  
  randomSeed(1);

  //Set the pin as an input
  pinMode(A0, INPUT);
  //Turn on the internal pullup
  digitalWrite(A0, HIGH);
  
  lastDrawTime = millis();
  led_setup();


  mainInit(newGrid);
  memcpy(oldGrid,newGrid,sizeof(newGrid));
}

void loop() {
  // If we haven't calculated our new grid for this draw, do it.
  
  if (needsUpdate) {
    mainUpdate(oldGrid, newGrid);
    memcpy(oldGrid,newGrid,sizeof(newGrid));
    cli();
    for (uint8_t i = 0; i < 5; i++) {
      for (uint8_t j = 0; j < 10; j++) {
        Pixel p = newGrid[i][j];
        send_pixel(p.r, p.g, p.b);
      }
    }
    sei();
    show_pixels();
    read_accel(&accel);
    Serial.println(accel.x);
    Serial.println(accel.y);
    Serial.println(accel.z);
    needsUpdate = false;
  }

  // Wait DRAW_OFFSET number of milliseconds between each draw, to prevent LED flicker.
  unsigned long curTime = millis();
  if (curTime > lastDrawTime + DRAW_OFFSET) {
    lastDrawTime = curTime;
    needsUpdate = true;
  }
}
