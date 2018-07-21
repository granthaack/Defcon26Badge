#ifndef DISPLAYS_H_   /* Include guard */
#define DISPLAYS_H_
#include "pixel.h"

unsigned int state = 0;

void sampleInit(struct Pixel newGrid[10][20]);
void sampleUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);
void scrollUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

void conwayInit(struct Pixel newGrid[10][20]);
void conwayUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

struct Display {
  void (*updateFunc) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);
  void (*initFunc) (struct Pixel newGrid[10][20]);
};

// We have 1kb of ROM, so shove everything we can in there.
// This stores all the functions we can use.
PROGMEM const struct Display displays[] =
{
  {&sampleUpdate,&sampleInit},
  {&scrollUpdate,&sampleInit},
  {&conwayUpdate,&conwayInit}
};

void sampleInit(struct Pixel newGrid[10][20]) {
  state = 0;
  memset(newGrid,0,sizeof(newGrid));
}

void sampleUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) {
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

void scrollUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) {
  
  uint8_t stateTable[] = {0,1,1,2,9,0,3,3,9,0,4,4,9,0,1,1,9,0,1,1,0,9,0,5,6,0,9,7,3,10,9,2,3,3,8,9,9,9,6,6,6,9,9,9};
  
  // Generator
  newGrid[5][19].g = ((letterSlices[state] << 0) & 1) * 150;
  newGrid[6][19].g = ((letterSlices[state] << 1) & 1) * 150;
  newGrid[7][19].g = ((letterSlices[state] << 2) & 1) * 150;
  newGrid[8][19].g = ((letterSlices[state] << 3) & 1) * 150;
  newGrid[9][19].g = ((letterSlices[state] << 4) & 1) * 150;

  // Fourth and third screens
  for(uint8_t i = 18; i >= 0; i--) {
    newGrid[5][i] = oldGrid[5][i+1];
    newGrid[6][i] = oldGrid[6][i+1];
    newGrid[7][i] = oldGrid[7][i+1];
    newGrid[8][i] = oldGrid[8][i+1];
    newGrid[9][i] = oldGrid[9][i+1];
  }
 
  // Wrap-around to second screen
  newGrid[0][19] = oldGrid[5][0];
  newGrid[1][19] = oldGrid[6][0];
  newGrid[2][19] = oldGrid[7][0];
  newGrid[3][19] = oldGrid[8][0];
  newGrid[4][19] = oldGrid[9][0];

  // First and second screen
  for(uint8_t i = 18; i >= 0; i--) {
    newGrid[5][i] = oldGrid[5][i+1];
    newGrid[6][i] = oldGrid[6][i+1];
    newGrid[7][i] = oldGrid[7][i+1];
    newGrid[8][i] = oldGrid[8][i+1];
    newGrid[9][i] = oldGrid[9][i+1];
  }

  state++;

  if(state > 44) {
    state = 0;
  }
}

void conwayInit(struct Pixel newGrid[10][20]) {
	memset(newGrid, 0, sizeof(newGrid));
	newGrid[1][4].g = 255;
	newGrid[1][5].g = 255;
	newGrid[2][3].g = 255;
	newGrid[2][4].g = 255;
	newGrid[3][4].g = 255;
}

void conwayUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]) {
	if (!digitalRead(A0)) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 20; j++) {
				if (random(0, 2)) {
					oldGrid[i][j].g = 255;
				}
			}
		}
	}

	

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			bool isAlive = oldGrid[i][j].g == 255;

			int numNeighbors = 0;
			if (i > 0) {
				numNeighbors += oldGrid[i - 1][j].g == 255;
				if (j > 0) {
					numNeighbors += oldGrid[i - 1][j - 1].g == 255;
				}
				if (j + 1 < 20) {
					numNeighbors += oldGrid[i - 1][j + 1].g == 255;

				}
			}
			if (i + 1 < 10) {
				
				numNeighbors += oldGrid[i + 1][j].g == 255;
				if (j > 0) {
					numNeighbors += oldGrid[i + 1][j - 1].g == 255;
				}
				if (j + 1 < 20) {
					numNeighbors += oldGrid[i + 1][j + 1].g == 255;

				}
			}
			if (j > 0) {
				numNeighbors += oldGrid[i][j - 1].g == 255;
			}
			if (j + 1 < 20) {
				numNeighbors += oldGrid[i][j + 1].g == 255;

			}
				
			if (!isAlive && numNeighbors == 3) newGrid[i][j].g = 255;
			else if (isAlive && numNeighbors <= 1) newGrid[i][j].g = 0;
			else if (isAlive && numNeighbors > 3) newGrid[i][j].g = 0;
			else newGrid[i][j].g = isAlive ? 255 : 0;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			newGrid[i][j].r = 0;
			newGrid[i][j].b = 0;
			if (oldGrid[i][j].g == 255 && newGrid[i][j].g != 255) newGrid[i][j].r = 255;
			if (oldGrid[i][j].r == 255 && newGrid[i][j].g != 255) newGrid[i][j].b = 255;
		}
	}
}
#endif // DISPLAYS_H_
