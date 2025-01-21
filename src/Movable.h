#pragma once

#include <QWidget>
#include <QPainter>

#include "common.h"
#include "Maze.h"
#include "Behaviour.h"

enum class Direction {
	Up,
	Right,
	Down,
	Left
};

class Movable : public QWidget {
	Q_OBJECT
public:
	Movable(Maze *parent);

	void paintEvent(QPaintEvent *event) override;

	double x = 2;
	double y = 1;
	Direction dir = Direction::Right;
	Direction intent = Direction::Down;
	double speed = 10;
	Behaviour *behaviour = nullptr;
public slots:
	void step();
private:
	Maze *maze;
};

class Player : public Movable {
public:
	Player(Maze *parent) : Movable(parent) {};
};

class Enemy : public Movable {
public:
	QColor color;

	Enemy(QColor color, Maze *parent) : Movable(parent) {
		this->color = color;
		speed = 7;
		x = y = 11;
	};

	void paintEvent(QPaintEvent *event) override;
};

