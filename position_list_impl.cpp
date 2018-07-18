#include "position_list.h"
#include "maze_set.h"
#include "maze_algorithm.h"


void PositionListInit(PositionList & _list)
{
	_list.m_pFirst = _list.m_pLast = nullptr;
}

void PositionListDestroy(PositionList & _list)
{
	PositionList::Node * pCurrent = _list.m_pFirst;
	while (pCurrent)
	{
		PositionList::Node * pTemp = pCurrent->m_pNext;
		delete pCurrent;
		pCurrent = pTemp;
	}

	_list.m_pFirst = _list.m_pLast = nullptr;
}

bool PositionListIsEmpty(PositionList & _list)
{
	return (_list.m_pFirst == nullptr);
}

void PositionListPushBack(PositionList & _list, int _x, int _y)
{
	PositionList::Node * pNewNode = new PositionList::Node;
	pNewNode->x = _x;
	pNewNode->y = _y;
	pNewNode->m_pNext = nullptr;

	if (!_list.m_pFirst)
		_list.m_pFirst = pNewNode;
	else
		_list.m_pLast->m_pNext = pNewNode;

	_list.m_pLast = pNewNode;
}

void PositionListPopFront(PositionList & _list)
{
	assert(!PositionListIsEmpty(_list));

	PositionList::Node * pFirst = _list.m_pFirst;
	if (_list.m_pFirst == _list.m_pLast)
	{
		_list.m_pFirst = _list.m_pLast = nullptr;
	}
	else
		_list.m_pFirst = pFirst->m_pNext;

	delete pFirst;
}
