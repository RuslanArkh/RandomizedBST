#-------------------------------------------------
#
# Project created by QtCreator 2015-06-04T21:04:36
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMazeGame
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    maze_algorithm_impl.cpp \
    maze_set_impl.cpp \
    position_list_impl.cpp \
    round.cpp

HEADERS  += widget.h \
    maze_algorithm.h \
    maze_set.h \
    position_list.h

FORMS    += widget.ui
