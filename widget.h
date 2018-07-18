#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>

#include "position_list.h"
#include "maze_algorithm.h"
#include "maze_set.h"

namespace Ui {
class Widget;
}

class MyRound;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    struct Room
    {
        int set;
        bool left;
        bool right;
        bool top;
        bool bottom;
    };
    struct MazeField
    {
        int m_matrixSize;
        int blockIndex;
        int cellIndex;

        Room **m_pDirectory;
    };

    class MyRound : public QGraphicsItem
    {
    public:
        MyRound(Widget::MazeField & pF);
        Widget::MazeField * f;
        QRectF boundingRect() const;
        void paint(QPainter * painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget);
        bool * winner;
    protected:
        unsigned int * _xPos, * _yPos;
        void keyPressEvent(QKeyEvent * event);
    };

    /////////////////////////////////////////////////////////////////////////////

    MazeField * pField;

    void MazeFieldDestroy(Widget::MazeField & _pField);

    void SetRightWalls(Widget::MazeField & _pField, MazeSet ** _Sets, MazeMap & s_bMap);
    void SetBottomWalls(Widget::MazeField & _pField, MazeSet ** _Sets, MazeMap & s_bMap);

    void PassOnNewRow(Widget::MazeField & _pField, MazeSet ** _Sets, MazeMap & s_bMap);	//s_bMap - Sets Bool Map
    void PassOnLastRow(Widget::MazeField & _pField, MazeMap & s_bMap);

    void BuildIt(int size);

private:
    Ui::Widget *ui;
    QGraphicsScene *scene;
    MyRound *walker;
    QGraphicsRectItem *rectangle;
    QGraphicsRectItem *exit;
    QGraphicsTextItem *text;
protected:
    Widget * field;
    MazeField * MazeFieldCreate(int size);
    void CreateMaze(Widget::MazeField & _pField, MazeSet ** _Sets);
};

#endif // WIDGET_H
