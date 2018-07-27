#ifndef DISPLAYS_H_   /* Include guard */
#define DISPLAYS_H_
#include <RF24.h>
#include "programs.h"

unsigned int state = 0;

void sampleInit(struct Pixel newGrid[10][20]);
void sampleUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);
void scrollUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

void conwayInit(struct Pixel newGrid[10][20]);
void conwayUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

void slaveInit(struct Pixel newGrid[10][20]);
void slaveUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

void menuInit(struct Pixel newGrid[10][20]);
void menuUpdate(struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);

struct Display {
  void (*updateFunc) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);
  void (*initFunc) (struct Pixel newGrid[10][20]);
};

// We have 1kb of ROM, so shove everything we can in there.
// This stores all the functions we can use.
const struct Display displays[] =
{
  {&sampleUpdate,&sampleInit},
  {&scrollUpdate,&sampleInit},
  {&conwayUpdate,&conwayInit}
};

void (*mainUpdate) (struct Pixel oldGrid[10][20], struct Pixel newGrid[10][20]);
void (*mainInit) (struct Pixel newGrid[10][20]);

#endif // DISPLAYS_H_
