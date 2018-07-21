#ifndef DISPLAYS_H_   /* Include guard */
#define DISPLAYS_H_
#include "pixel.h"

void sampleInit(struct Pixel newGrid[10][20]);
void sampleUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

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
  {&conwayUpdate,&conwayInit}
};

void sampleInit(struct Pixel newGrid[10][20]) {
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