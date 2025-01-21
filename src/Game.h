#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <vector>

#include "TopBar.h"
#include "Maze.h"
#include "BottomBar.h"
#include "Movable.h"

class Game : public QWidget {
	Q_OBJECT
private:
	TopBar *tbar = new TopBar(this);
	BottomBar *bbar = new BottomBar(this);
public:
	Maze *maze = new Maze(this);
	Player *player = new Player(maze);
	std::vector<Movable *> movables = {player};

	Game();
protected:
	void keyPressEvent(QKeyEvent *event) override;
};

