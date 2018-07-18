#include "widget.h"
#include "ui_widget.h"
#include "maze_algorithm.h"
#include "maze_set.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    const int size = 30;

    ui->setupUi(this);
//    field = new Widget();
    BuildIt(size);
    scene = new QGraphicsScene();

    ui->graphicsView->setScene(scene);

    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    rectangle = scene->addRect(0,0,600,600,blackPen);

    for (int i = 0, xR = 20, yR = 0, xD = 0, yD = 20; i < 30; i++, yR += 20, yD += 20, xR = 20, xD = 0)
    {
        for (int j = 0; j < 30; j++, xR += 20, xD += 20)
        {
            if (pField->m_pDirectory[i][j].right)
                scene->addLine(xR,yR,xR,yR + 20,blackPen);
            if (pField->m_pDirectory[i][j].bottom)
                scene->addLine(xD,yD,xD + 20,yD,blackPen);
        }
    }

    QPen exitPen(Qt::green);
    exitPen.setWidth(0);
    QBrush brushEx(Qt::green);

    exit = scene->addRect(581,261,17,17,exitPen,brushEx);

    walker = new Widget::MyRound(*pField);
    scene->addItem(walker);
}

Widget::~Widget()
{
    MazeFieldDestroy(*pField);
    delete ui;
}

void Widget::BuildIt(int size)
{
    pField = MazeFieldCreate(size);
    MazeSet ** pSets = new MazeSet*[size];

    MazeSetsInitializer(pSets, size);
    RandomInitializer();

    CreateMaze(*pField, pSets);
    MazeSetsDestroy(pSets, size);
}
