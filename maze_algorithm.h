#ifndef _MAZE_H_
#define _MAZE_H_

#include <stdlib.h>
#include <ctime>
#include <cassert>

typedef unsigned short int USHORT;

struct MazeSet;

struct MazeMap;

MazeMap * MazeMapCreate(int _size);

void MazeSetsInitializer(MazeSet ** _Sets, unsigned short int _size);

void RandomInitializer();
unsigned short int RandomGenerator(unsigned short int min, unsigned short int max);

#endif
