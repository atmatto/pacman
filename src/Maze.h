#pragma once

#include <QWidget>

#include "MazeCell.h"

class Entity;

class Maze : public QWidget {
	Q_OBJECT
public:
	explicit Maze(const Maze *maze = nullptr, QWidget *parent = nullptr);

	void resetCells();

	MazeCell cell(int x, int y);
	void setCell(int x, int y, MazeCell cell);
	bool cellSolid(int x, int y);
	bool hasConsumables();

	inline const static int WIDTH = 28;
	inline const static int HEIGHT = 31;
public slots:
	void repositionEntity(Entity &e);
signals:
	void entityRepositioningRequired();
protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
private:
	MazeCell cells[HEIGHT][WIDTH] = {};
};
