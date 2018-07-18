#include "widget.h"

struct Room
{
    unsigned short set : 6;
    bool left;
    bool right;
    bool top;
    bool bottom;
};
struct MazeField
{
    Room **m_pDirectory;
};

Widget::MyRound::MyRound(Widget::MazeField & pF)
{
    f = new MazeField;
    f->m_matrixSize = pF.m_matrixSize;
    f->m_pDirectory = new Widget::Room*[f->m_matrixSize];
    for (int i = 0; i < f->m_matrixSize; i++)
        f->m_pDirectory[i] = new Widget::Room[f->m_matrixSize];

    for (USHORT i = 0; i < 30; i++)
    {
        for (USHORT j = 0; j < 30; j++)
        {
            f->m_pDirectory[i][j] = pF.m_pDirectory[i][j];
        }
    }
    winner = new bool;
    *winner = false;
    _xPos = new unsigned int;
    _yPos = new unsigned int;
    *_xPos = 0;
    *_yPos = 13;
    setFlag(ItemIsFocusable);
    setFocus();
}

QRectF Widget::MyRound::boundingRect() const
{
    return QRectF(2,262,15,15);
}

void Widget::MyRound::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF r = boundingRect();
    QPen penR(Qt::red);
    penR.setWidth(2);
    painter->setPen(penR);
    QBrush brushR(Qt::red);
    painter->setBrush(brushR);

    painter->drawEllipse(r);

}

void Widget::MyRound::keyPressEvent(QKeyEvent *event)
{
    if (*winner)
        return;
    if (event->key() == Qt::Key_Left)
    {
        if (!f->m_pDirectory[*_yPos][*_xPos].left)
        {
            --*_xPos;
            setPos(x()-20,y());
        }
    }
    else if (event->key() == Qt::Key_Right)
    {
        if (!f->m_pDirectory[*_yPos][*_xPos].right)
        {
            ++*_xPos;
            setPos(x()+20,y());
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (!f->m_pDirectory[*_yPos][*_xPos].top)
        {
            --*_yPos;
            setPos(x(),y()-20);
        }
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (!f->m_pDirectory[*_yPos][*_xPos].bottom)
        {
            ++*_yPos;
            setPos(x(),y()+20);
        }
    }
    if (*_xPos == 29 && *_yPos == 13)
    {
        *winner = true;
        QPainterPath path;
        QFont font;
        font.setPixelSize(60);
        font.setBold(true);
        font.setFamily("Calibri");
        path.addText(45,280,font,"CONGRATULATIONS!");

        Widget::MyRound::scene()->addPath(path, QPen(QBrush(QColor(255, 102, 0, 255)), 1), QBrush(QColor(255, 102, 0, 255)));
    }

}
