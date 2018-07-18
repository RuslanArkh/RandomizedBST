#ifndef _MAZE_LIST_H_
#define _MAZE_LIST_H_
#include <cassert>

struct PositionList
{
	struct Node
	{
        int x;
        int y;
		Node * m_pNext;
	};
	Node * m_pFirst, *m_pLast;
};

void PositionListInit(PositionList & _list);

void PositionListDestroy(PositionList & _list);

bool PositionListIsEmpty(PositionList & _list);

void PositionListPushBack(PositionList & _list, int _x, int _y);

void PositionListPopFront(PositionList & _list);

unsigned short int PositionListSize(PositionList & _list);

#endif
