TEMPLATE = app
TARGET = pacman
# CONFIG += debug

QT += widgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables APIs deprecated before Qt 6.0.0

RESOURCES = res/res.qrc

# find src -name '*.cpp' | tr '\n' ' ' | sed 's/^/SOURCES += /;a\'
# find src -name '*.h' | tr '\n' ' ' | sed 's/^/HEADERS += /;a\'

SOURCES += src/Behaviour.cpp src/Entity.cpp src/Window.cpp src/Maze.cpp src/BottomBar.cpp src/Enemy.cpp src/TopBar.cpp src/Maze.resetCells.cpp src/ValueDisplay.cpp src/Round.cpp src/Game.cpp src/Player.cpp src/Menu.cpp src/Timer.cpp src/Images.cpp
HEADERS += src/Round.h src/BottomBar.h src/Behaviour.h src/Timer.h src/Window.h src/Menu.h src/MazeCell.h src/Direction.h src/Entity.h src/ValueDisplay.h src/Enemy.h src/Game.h src/DistanceField.h src/Player.h src/TopBar.h src/Maze.h src/Images.h
