#ifndef MENU_H_
#define MENU_H_
#include "programs.h"

extern unsigned int state;
void clearGrid(struct Pixel grid[10][20]);

bool isMenu = false;
bool isMaster = true;
uint8_t program;
uint8_t MAX_PROGRAM = 2;

RF24 radio(7,8);
uint8_t id = 0;

void menuUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);
void menuInit(struct Pixel newGrid[10][20]);
void slaveInit(struct Pixel newGrid[10][20]);
void slaveUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

void menuInit(struct Pixel newGrid[10][20]) {
  state = 0;
  id = 1;
  clearGrid(newGrid);
}

void menuUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) {
  clearGrid(newGrid);

  // State
  // Maintain state so that buttons are only registered as
  // "pressed" on a key up.
  // 0000_0NPM
  // M - Master/Slave Mode
  // P - Program
  // N - Number of connected devices (inclusive)

  // Master/Slave Mode (Upper Left)
  if(!digitalRead(A3)) {
    state = state | 0b1;
  }
  if((state & 0b1) && digitalRead(A3)) {
    id = 1;
    isMaster = !isMaster;
    state = state & 0b11111110;
  }

  // Program button (Middle Right)
  if(!digitalRead(A1)) {
      state = state | 0b10;
  }

  if((state & 0b10) && digitalRead(A1)) {
    state = state & 0b11111101;
    program++;
    program = program % (MAX_PROGRAM + 1);
  }

  // Number of connected devices (Upper Right)
  // Or in slave mode, device ID
  if(!digitalRead(A0)) {
    state = state | 0b100;
  }
  if((state & 0b100) && digitalRead(A0)) {
    if(isMaster) {
      id = (id == 4) ? 1 : id << 1;
    } else {
      id = (id == 3) ? 1 : id + 1;
    }
    state = state & 0b11111011;
  }

  // Show number of connected devices
  if(isMaster) {
    switch(id) {
      case 4:
        newGrid[2][1].r = 200;
        newGrid[2][0].r = 200;
      case 2:
        newGrid[1][1].r = 200;
      case 1:
        newGrid[1][0].r = 200;
        break;
    }    
  } else {
    newGrid[1][9].r = newGrid[1][8].r = newGrid[1][7].r = newGrid[1][6].r = 200;
    newGrid[1][9].g = newGrid[1][8].g = newGrid[1][7].g = newGrid[1][6].g = 200;
    newGrid[1][9].b = newGrid[1][8].b = newGrid[1][7].b = newGrid[1][6].b = 200;
    switch(id) {
      case 3:
        newGrid[1][9].r = 0;
        newGrid[1][9].g = 0;
        break;
      case 2:
        newGrid[1][8].r = 0;
        newGrid[1][8].g = 0;
        break;
      case 1:
        newGrid[1][7].r = 0;
        newGrid[1][7].g = 0;
        break;
    }
  }

  

  for(uint8_t i = 0; i < 10; i++) {
    newGrid[4][i].b = newGrid[4][i].g = newGrid[4][i].r = 0;
    if(isMaster) {
        newGrid[4][i].b = 255;
    } else {
      newGrid[4][i].g = 255;
    }
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
