#pragma once

#include <QWidget>

#include "Direction.h"

class Maze;

// Base class for the player and enemies.
class Entity : public QWidget {
	Q_OBJECT
public:
	explicit Entity(QWidget *parent = nullptr);

	double x = -1;
	double y = -1;

	Direction dir = Direction::Left;
	Direction intent = Direction::Left;

	double speed = 10;
	double baseSpeed = 10;
public slots:
	// Calculate the new position after deltaTime seconds have elapsed.
	void step(Maze &maze, double deltaTime);
signals:
	void positionChanged();
};