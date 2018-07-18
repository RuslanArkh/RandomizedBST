#include "maze_set.h"
#include "position_list.h"
#include "maze_algorithm.h"

struct MazeSet
{
    PositionList m_data;
};

MazeSet * MazeSetCreate()
{
	MazeSet * pSet = new MazeSet;
	return pSet;
}

void MazeSetsDestroy(MazeSet ** _pSet, USHORT _size)
{
	for (USHORT i = 0; i < _size; i++)
		PositionListDestroy(_pSet[i]->m_data);
	delete[] _pSet;
}

void MazeSetInsertKey(MazeSet ** _set, USHORT _number, USHORT coordX, USHORT coordY)
{
	PositionListPushBack(_set[_number]->m_data, coordX, coordY);
}

bool MazeSetIsEmpty(MazeSet ** _set, USHORT _number)
{
	return PositionListIsEmpty(_set[_number]->m_data);
}

void MazeSetDeleteFirst(MazeSet ** _set, USHORT _number)
{
	PositionListPopFront(_set[_number]->m_data);
}
