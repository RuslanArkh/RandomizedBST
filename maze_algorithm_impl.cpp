#include "maze_algorithm.h"
#include "position_list.h"
#include "maze_set.h"
#include "widget.h"

struct MazeMap
{
    bool * maps;
    struct stateOfSet
    {
        unsigned short once : 1;
        unsigned short amountOfCycles : 4;
        enum { NORMAL, CYCLEPOSSIBLE, LITTLECYCLE } m_status;
    };
    stateOfSet * mInfo;
};

struct MazeSet
{
    PositionList m_data;
    signed short CellsInSet : 7;
    signed short AmountOfBottom : 7;
};

//////////////////////////////////////////////////////////////////////////////

Widget::MazeField * Widget::MazeFieldCreate(int m_Size)
{
	assert(m_Size > 0);

    Widget::MazeField * pNewMazeField = new Widget::MazeField;

	pNewMazeField->m_matrixSize = m_Size;

    pNewMazeField->m_pDirectory = new Widget::Room*[pNewMazeField->m_matrixSize];
	for (int i = 0; i < pNewMazeField->m_matrixSize; i++)
        pNewMazeField->m_pDirectory[i] = new Widget::Room[pNewMazeField->m_matrixSize];

    for (int i = 0; i < pNewMazeField->m_matrixSize; i++)
        for (int j = 0; j < pNewMazeField->m_matrixSize; j++)
		{
			i == 0 ?
				pNewMazeField->m_pDirectory[i][j].top = true
				:
				pNewMazeField->m_pDirectory[i][j].top = false;

			i == pNewMazeField->m_matrixSize - 1 ?
				pNewMazeField->m_pDirectory[i][j].bottom = true
				:
				pNewMazeField->m_pDirectory[i][j].bottom = false;

			j == pNewMazeField->m_matrixSize - 1 ?
				pNewMazeField->m_pDirectory[i][j].right = true
				:
				pNewMazeField->m_pDirectory[i][j].right = false;

			j == 0 ?
				pNewMazeField->m_pDirectory[i][j].left = true
				:
				pNewMazeField->m_pDirectory[i][j].left = false;
			if (i == 0)
				pNewMazeField->m_pDirectory[i][j].set = j;
			else
				pNewMazeField->m_pDirectory[i][j].set = pNewMazeField->m_matrixSize;
		}
	pNewMazeField->blockIndex = 0;
	pNewMazeField->cellIndex = 0;
	return pNewMazeField;
}
void Widget::MazeFieldDestroy(Widget::MazeField & _pField)
{
	for (USHORT i = 0; i < _pField.m_matrixSize; i++)
		delete[] _pField.m_pDirectory[i];
	delete[] _pField.m_pDirectory;
	delete &_pField;
}

MazeMap * MazeMapCreate(int _size)
{
	MazeMap * SetsBoolMap = new MazeMap;
    SetsBoolMap->maps = new bool[_size];

    SetsBoolMap->mInfo = new MazeMap::stateOfSet[_size];
	for (USHORT i = 0; i < _size; i++)
	{
        SetsBoolMap->maps[i] = false;
        SetsBoolMap->mInfo[i].m_status = MazeMap::stateOfSet::NORMAL;
        SetsBoolMap->mInfo[i].amountOfCycles = 0;
        SetsBoolMap->mInfo[i].once = 0;
	}
	return SetsBoolMap;
}

void MazeSetsInitializer(MazeSet ** _Sets, unsigned short int _size)
{
	for (unsigned short int i = 0; i < _size; i++)
	{
		_Sets[i] = MazeSetCreate();
        _Sets[i]->CellsInSet = 1;
		PositionListInit(_Sets[i]->m_data);
	}
}

void RandomInitializer()
{
	time_t currentTime;
	time(&currentTime);
	srand((unsigned)currentTime);
}

unsigned short int RandomGenerator(unsigned short int min, unsigned short int max)
{
	return rand() % (max - min + 1) + min;
}

void MazeInitialBorders(Widget::MazeField & _pField, USHORT _size)
{
	for (USHORT i = 0; i < _size; i++)
	{
		_pField.m_pDirectory[0][i].top = true;
		_pField.m_pDirectory[_size - 1][i].bottom = true;

		_pField.m_pDirectory[i][0].left = true;
		_pField.m_pDirectory[i][_size - 1].right = true;
	}
}

void Widget::SetRightWalls(Widget::MazeField & _pField, MazeSet ** _Sets, MazeMap & s_bMap)
{
	USHORT randomRight;
	for (_pField.cellIndex = 0; _pField.cellIndex < _pField.m_matrixSize - 1; _pField.cellIndex++)
	{
		if (_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set == _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set)
		{
			_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].right = true;
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].left = true;
		}
	}
    _Sets[_pField.m_pDirectory[_pField.blockIndex][0].set]->CellsInSet = 1;
	for (_pField.cellIndex = 0; _pField.cellIndex < _pField.m_matrixSize - 1; _pField.cellIndex++)
	{

        ++_Sets[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set]->CellsInSet;

        if (_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set == _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set)
        {
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].right = true;
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].left = true;
            continue;
        }
        else if (s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set].m_status != MazeMap::stateOfSet::NORMAL
                 && _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set != _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set)
        {
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].right = true;
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].left = true;
            continue;
        }


		randomRight = RandomGenerator(0, 100);
        if (randomRight >= (_pField.blockIndex < 2 ? 40 : 70)
                && !_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].right)
        {
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].right = true;
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].left = true;

        }
        else if (randomRight < (_pField.blockIndex < 2 ? 40 : 70))
        {
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set = _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set;
            ++_Sets[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set]->CellsInSet;
        }
	}
	_pField.cellIndex = 0;
	for (int i = 0; i < _pField.m_matrixSize; i++)
    {
        _pField.cellIndex = 0;
		if (_Sets[i]->CellsInSet > 1)
		{
            unsigned short int size = _Sets[i]->CellsInSet;
            while (size > 1)
			{
				if (_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set == i)
				{
					--size;
					MazeSetInsertKey(_Sets, _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set, _pField.cellIndex, _pField.blockIndex);
                }
                ++_pField.cellIndex;
                if (_pField.cellIndex >= _pField.m_matrixSize)
                    _pField.cellIndex = 0;
			}
		}
    }
}

void Widget::SetBottomWalls(Widget::MazeField & _pField, MazeSet ** _Sets, MazeMap & s_bMap)
{
    for (USHORT i = 0, randomBottom; i < _pField.m_matrixSize; i++)
    {
        if (!MazeSetIsEmpty(_Sets, i))
        {
            _Sets[i]->AmountOfBottom = 0;
            USHORT k = 0;
            USHORT x1, x2;
            x1 = _Sets[i]->m_data.m_pFirst->x;
            while (!MazeSetIsEmpty(_Sets, i))
            {
                randomBottom = RandomGenerator(0, 100);
                ++k;
                if (randomBottom < (s_bMap.mInfo[i].m_status == MazeMap::stateOfSet::CYCLEPOSSIBLE ? 30 : 50))
                {
                    _pField.m_pDirectory[_Sets[i]->m_data.m_pFirst->y][_Sets[i]->m_data.m_pFirst->x].bottom = true;
                    _pField.m_pDirectory[_Sets[i]->m_data.m_pFirst->y + 1][_Sets[i]->m_data.m_pFirst->x].top = true;
                    ++_Sets[i]->AmountOfBottom;
                }

                if (k >= _Sets[i]->CellsInSet - 1)
                {
                    x2 = _Sets[i]->m_data.m_pFirst->x + 1;
                }
                MazeSetDeleteFirst(_Sets, i);
            }
            if (x2 >= _pField.m_matrixSize)
                --x2;
            if (x2 >= _pField.m_matrixSize - 1)
            {
                USHORT rightBottom = RandomGenerator(0, 100);
                if (rightBottom >= (s_bMap.mInfo[i].m_status == MazeMap::stateOfSet::CYCLEPOSSIBLE ? 0 : 40))
                {

                    _pField.m_pDirectory[_pField.blockIndex][x2].bottom = true;
                    _pField.m_pDirectory[_pField.blockIndex + 1][x2].top = true;
                    ++_Sets[i]->AmountOfBottom;
                }
            }

            //  Если нижних границ получилось столько же, сколько ячеек в множестве, то пробиваем стену
            if (_Sets[i]->AmountOfBottom >= _Sets[i]->CellsInSet
                    &&
                    s_bMap.mInfo[i].m_status == MazeMap::stateOfSet::NORMAL)
            {
                USHORT pos = RandomGenerator(0, _Sets[i]->CellsInSet );
                _pField.m_pDirectory[_pField.blockIndex][x1].bottom = false;
                _pField.m_pDirectory[_pField.blockIndex + 1][x1].top = false;
                --_Sets[i]->AmountOfBottom;
                continue;
            }

            //  Если нижних границ на одну меньше, чем ячеек, а до этого множество было с возможностью цикла, то множество теперь нормальное
            if (_Sets[i]->AmountOfBottom >= _Sets[i]->CellsInSet - 1
                    &&
                    s_bMap.mInfo[i].m_status == MazeMap::stateOfSet::CYCLEPOSSIBLE)
            {
                if (_Sets[i]->AmountOfBottom == _Sets[i]->CellsInSet)
                {
                    _pField.m_pDirectory[_pField.blockIndex][x1].bottom = false;
                    _pField.m_pDirectory[_pField.blockIndex + 1][x1].top = false;
                    --_Sets[i]->AmountOfBottom;
                }
                s_bMap.mInfo[i].m_status = MazeMap::stateOfSet::NORMAL;
            }


            if (_Sets[i]->CellsInSet >= 2 && _Sets[i]->AmountOfBottom <= _Sets[i]->CellsInSet - 2
                    &&
                    s_bMap.mInfo[i].m_status == MazeMap::stateOfSet::NORMAL)
            {
                for (USHORT m = x1; m < x2; m++)
                {
                    if (!_pField.m_pDirectory[_pField.blockIndex][m].bottom
                            &&
                            _pField.m_pDirectory[_pField.blockIndex][m + 1].bottom)
                    {
                        ++m;
                        while (_pField.m_pDirectory[_pField.blockIndex][m].bottom)
                        {

                            ++m;
                            if (!_pField.m_pDirectory[_pField.blockIndex][m].bottom)
                            {
                                s_bMap.mInfo[i].m_status = MazeMap::stateOfSet::CYCLEPOSSIBLE;
                                break;
                            }
                        }
                    }
                }
                for (USHORT m = x1; m < x2; m++)
                {
                    if (!_pField.m_pDirectory[_pField.blockIndex][m].bottom
                            &&
                            !_pField.m_pDirectory[_pField.blockIndex][m + 1].bottom
                            &&
                            !_pField.m_pDirectory[_pField.blockIndex][m].right)
                    {
                        s_bMap.mInfo[i].m_status = MazeMap::stateOfSet::CYCLEPOSSIBLE;
                        break;
                    }
                }
            }

        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void Widget::PassOnNewRow(Widget::MazeField & _pField, MazeSet ** _Sets, MazeMap & s_bMap)
{
    if (_pField.blockIndex == _pField.m_matrixSize - 1)
        for (USHORT i = 0; i < _pField.m_matrixSize; i++)
            s_bMap.maps[i] = false;
	int boolCount = 0;
	for (_pField.cellIndex = 0; _pField.cellIndex < _pField.m_matrixSize; _pField.cellIndex++)
	{
		if (!_pField.m_pDirectory[_pField.blockIndex - 1][_pField.cellIndex].bottom)
		{
			_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set = _pField.m_pDirectory[_pField.blockIndex - 1][_pField.cellIndex].set;
			if (!s_bMap.maps[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set])
			{
				boolCount++;
				s_bMap.maps[_pField.m_pDirectory[_pField.blockIndex - 1][_pField.cellIndex].set] = true;
			}
		}
	}
    if (boolCount == _pField.m_matrixSize)
		return;
	//	Assigns another unrelated sets to free sets according to the bool map
	_pField.cellIndex = 0;
	for (unsigned short int setNumber = 0; _pField.cellIndex < _pField.m_matrixSize; _pField.cellIndex++)
	{
		while (s_bMap.maps[setNumber])
		{
			if (setNumber >= _pField.m_matrixSize - 1)
				setNumber = 0;
			else
				++setNumber;
		}
		assert(setNumber < _pField.m_matrixSize);
        assert(_pField.cellIndex < _pField.m_matrixSize);

		if (_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].top)
		{
			_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set = setNumber;
			boolCount++;
			s_bMap.maps[setNumber] = true;
		}
		if (boolCount > _pField.m_matrixSize - 1)
			break;
	}
}

void NullMaps(Widget::MazeField & _pField, MazeSet ** _Sets, MazeMap & s_bMap)
{
	for (USHORT i = 0; i < _pField.m_matrixSize; i++)
	{
		s_bMap.maps[i] = false;
		_Sets[i]->AmountOfBottom = 0;
        _Sets[i]->CellsInSet = 0;
	}
}

void Widget::PassOnLastRow(Widget::MazeField & _pField, MazeMap & s_bMap)
{
    // Подсчитываем возможное количесвто циклов
    for (_pField.cellIndex = 0; _pField.cellIndex < _pField.m_matrixSize - 1; _pField.cellIndex++)
    {
        if (s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].m_status == MazeMap::stateOfSet::CYCLEPOSSIBLE
            && _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set != _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set)
        {
            if (s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].amountOfCycles == 0)
            {
                ++s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].amountOfCycles;
                ++s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].once;
            }
            else if (s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].once == 1)
            {
                s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].once = 0;
            }
            else
            {
                ++s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].amountOfCycles;
            }
        }
    }
    //  Строим границы между двумя одинаковыми множествами
    for (_pField.cellIndex = 0; _pField.cellIndex < _pField.m_matrixSize - 1; _pField.cellIndex++)
    {
        if (_pField.m_pDirectory[_pField.blockIndex - 1][_pField.cellIndex].set == _pField.m_pDirectory[_pField.blockIndex - 1][_pField.cellIndex + 1].set
            &&
            !_pField.m_pDirectory[_pField.blockIndex - 1][_pField.cellIndex].bottom
            &&
            !_pField.m_pDirectory[_pField.blockIndex - 1][_pField.cellIndex + 1].bottom)
        {
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].right = true;
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].left = true;
            if (s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].once == 1 &&
                s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].amountOfCycles == 1)
                s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex - 1][_pField.cellIndex].set].m_status = MazeMap::stateOfSet::NORMAL;
        }
        if (_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set != _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set
            &&
            (s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].m_status == MazeMap::stateOfSet::NORMAL
             && s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set].m_status == MazeMap::stateOfSet::NORMAL)
                && s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set].once == 0)
        {
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].right = false;
            _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].left = false;
        }
    }
    //  Перекрываем циклы
	for (_pField.cellIndex = 0; _pField.cellIndex < _pField.m_matrixSize - 1; _pField.cellIndex++)
    {
        if (s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].m_status == MazeMap::stateOfSet::CYCLEPOSSIBLE
                && _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set != _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].set)
        {
            if (s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].amountOfCycles > 0
                    && s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].once != 1)
            {
                _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].right = true;
                _pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex + 1].left = true;
                --s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].amountOfCycles;
            }
            else
                s_bMap.mInfo[_pField.m_pDirectory[_pField.blockIndex][_pField.cellIndex].set].m_status = MazeMap::stateOfSet::NORMAL;
        }
	}
}

void Widget::CreateMaze(Widget::MazeField & _pField, MazeSet ** _Sets)
{
	MazeMap * pMap = MazeMapCreate(_pField.m_matrixSize);
	while (_pField.blockIndex < _pField.m_matrixSize)
	{

		if (_pField.blockIndex > 0 && _pField.blockIndex < _pField.m_matrixSize - 1)
		{
            NullMaps(_pField, _Sets, *pMap);
			PassOnNewRow(_pField, _Sets, *pMap);
			SetRightWalls(_pField, _Sets, *pMap);
			SetBottomWalls(_pField, _Sets, *pMap);
		}
		else if (_pField.blockIndex == _pField.m_matrixSize - 1)
		{
            PassOnNewRow(_pField, _Sets, *pMap);
			PassOnLastRow(_pField, *pMap);
		}
		else
		{
            NullMaps(_pField, _Sets, *pMap);
			SetRightWalls(_pField, _Sets, *pMap);
			SetBottomWalls(_pField, _Sets, *pMap);
        }
        ++_pField.blockIndex;
    }
}

///////////////////////////////////////////////////////////////////////////////
