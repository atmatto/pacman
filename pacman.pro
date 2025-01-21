TEMPLATE = app
TARGET = pacman
CONFIG += debug

QT += widgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables APIs deprecated before Qt 6.0.0

# ls src | grep .h | sed 's:^:src/:' | tr '\n' ' ' | sed -e 'a\'
# ls src | grep .cpp | sed 's:^:src/:' | tr '\n' ' ' | sed -e 'a\'

HEADERS += src/Behaviour.h src/BottomBar.h src/common.h src/Field.h src/Game.h src/LivesDisplay.h src/Maze.h src/Movable.h src/ScoreDisplay.h src/TopBar.h
SOURCES += src/Behaviour.cpp src/cells.cpp src/Game.cpp src/main.cpp src/Maze.cpp src/Movable.cpp
