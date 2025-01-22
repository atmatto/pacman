#pragma once

#include <QWidget>

#include "Direction.h"

class Maze;

class Entity : public QWidget {
	Q_OBJECT
public:
	explicit Entity(QWidget *parent = nullptr);

	double x = 2;
	double y = 1;

	Direction dir = Direction::Left;
	Direction intent = Direction::Left;

	double speed = 10;
public slots:
	void step(Maze &maze, double deltaTime);
signals:
	void positionChanged();
protected:
	void paintEvent(QPaintEvent *event) override;
private:
	Maze *maze;
};