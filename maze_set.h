#ifndef _MAZE_SET_H_
#define _MAZE_SET_H_

struct MazeSet;
struct MazeField;
struct Room;

typedef unsigned short int USHORT;

MazeSet * MazeSetCreate();

void MazeSetsInitialFill(MazeField * _pField, MazeSet ** _Sets);

void MazeSetsDestroy(MazeSet ** _pSet, unsigned short int _size);

void MazeSetInsertKey(MazeSet ** _set, USHORT _number, USHORT coordX, USHORT coordY);

void MazeSetDeleteFirst(MazeSet ** _set, USHORT _number);

bool MazeSetIsEmpty(MazeSet ** _set, USHORT _number);

USHORT MazeSetSize(MazeSet ** _set, USHORT _number);

void MazeSetClear(MazeSet ** _set, USHORT _number);

USHORT MazeSetGetLastX(MazeSet ** _set, USHORT _number);

#endif
