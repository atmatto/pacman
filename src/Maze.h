#pragma once

#include <QWidget>
#include <QTimer>

class Movable;
class Game;

enum class MazeCell {
	Empty,
	Wall,
	Dot,
	Energizer
};

class Maze : public QWidget {
	Q_OBJECT
public:
	// Movable *test;
	Game *game;

	Maze(Game *parent);
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

	MazeCell cell(int x, int y);
	bool cellSolid(int x, int y);
	void positionMovables(std::vector<Movable *> *movables);

	inline const static int WIDTH = 28;
	inline const static int HEIGHT = 31;
	MazeCell cells[HEIGHT][WIDTH] = {};

	void resetCells();
private slots:
	// void update();
};
