#ifndef MENU_H_   /* Include guard */
#define MENU_H_
#include "programs.h"

extern unsigned int state;
void clearGrid(struct Pixel grid[10][20]);

bool isMenu = false;
bool isMaster = true;
uint8_t program;
uint8_t MAX_PROGRAM = 2;

void menuUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);
void menuInit(struct Pixel newGrid[10][20]);
void slaveInit(struct Pixel newGrid[10][20]);
void slaveUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);


//void (*mainUpdate) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) = &menuUpdate;
//void (*mainInit) (struct Pixel newGrid[10][20]) = &menuInit;

void menuInit(struct Pixel newGrid[10][20]) {
  state = 0;
  clearGrid(newGrid);
}

void menuUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) {
  clearGrid(newGrid);
  if(!digitalRead(A1)) {
    state = 1;
  }

  if(!digitalRead(A3)) {
    state = 2;
  }
  if(state == 2 && digitalRead(A3)) {
    isMaster = !isMaster;
    state = 0;
  }

  for(uint8_t i = 0; i < 10; i++) {
    newGrid[4][i].b = newGrid[4][i].g = newGrid[4][i].r = 0;
    if(isMaster) {
        newGrid[4][i].b = 255;
    } else {
      newGrid[4][i].g = 255;
    }
  }

  if(state == 1 && digitalRead(A1)) {
    state = 0;
    program++;
    program = program % (MAX_PROGRAM + 1);
  }

  if(isMaster) {
    // Paint menu
    for(uint8_t i = 0; i < (MAX_PROGRAM + 1); i++) {
      if(i == program) {
        newGrid[0][i].b = newGrid[0][i].r = 0;
        newGrid[0][i].g = 255; 
      } else {
        newGrid[0][i].b = newGrid[0][i].g = newGrid[0][i].r = 155;
      }
    }
  }

  if(!digitalRead(A2)) {
    if(isMaster) {
      isMenu = false;
      state = 0;
      mainInit = displays[program].initFunc;
      clearGrid(newGrid);
      mainInit(newGrid);
      mainUpdate = displays[program].updateFunc;      
    } else {
      mainInit = slaveInit;
      slaveInit(newGrid);
    }
  }
}

void slaveInit(struct Pixel newGrid[10][20]) {

}

struct Message {
  struct Pixel grid[5][10];
};

void sendGrid(struct Pixel newGrid[10][20]) {

}

void slaveUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) {

}
#endif // MENU_H_
