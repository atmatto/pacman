#include <cmath>

#include "Movable.h"

Movable::Movable(Maze *parent) : QWidget(dynamic_cast<QWidget *>(parent)), maze(parent) {}

void Movable::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setBrush(QColor("#ffff00"));
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(0, 0, width(), width());
}

void Movable::step() {
	if (behaviour)
		intent = behaviour->getIntent(*maze->game, *this);

	int cx = std::round(x);
	int cy = std::round(y);
	double distance = speed / FPS;
	
	if (dir == Direction::Up) {
		int ny = std::floor(y);
		if (maze->cellSolid(cx, ny)) {
			ny += 1;
		}
		if (intent == Direction::Down) {
			dir = Direction::Down;
		} else if (intent == Direction::Left) {
			if (!maze->cellSolid(cx - 1, ny) && y - ny <= distance) {
				y = ny;
				distance -= y - ny;
				dir = Direction::Left;
			}
		} else if (intent == Direction::Right) {
			if (!maze->cellSolid(cx + 1, ny) && y - ny <= distance) {
				y = ny;
				distance -= y - ny;
				dir = Direction::Right;
			}
		}
	} else if (dir == Direction::Right) {
		int nx = std::ceil(x);
		if (maze->cellSolid(nx, cy)) {
			nx -= 1;
		}
		if (intent == Direction::Left) {
			dir = Direction::Left;
		} else if (intent == Direction::Up) {
			int nx = std::ceil(x);
			if (!maze->cellSolid(nx, cy - 1) && nx - x <= distance) {
				x = nx;
				distance -= x - nx;
				dir = Direction::Up;
			}
		} else if (intent == Direction::Down) {
			int nx = std::ceil(x);
			if (!maze->cellSolid(nx, cy + 1) && nx - x <= distance) {
				x = nx;
				distance -= x - nx;
				dir = Direction::Down;
			}
		}
	} else if (dir == Direction::Down) {
		int ny = std::ceil(y);
		if (maze->cellSolid(cx, ny)) {
			ny -= 1;
		}
		if (intent == Direction::Up) {
			dir = Direction::Up;
		} else if (intent == Direction::Left) {
			if (!maze->cellSolid(cx - 1, ny) && ny - y <= distance) {
				y = ny;
				distance -= y - ny;
				dir = Direction::Left;
			}
		} else if (intent == Direction::Right) {
			if (!maze->cellSolid(cx + 1, ny) && ny - y <= distance) {
				y = ny;
				distance -= y - ny;
				dir = Direction::Right;
			}
		}
	} else if (dir == Direction::Left) {
		int nx = std::floor(x);
		if (maze->cellSolid(nx, cy)) {
			nx += 1;
		}
		if (intent == Direction::Right) {
			dir = Direction::Right;
		} else if (intent == Direction::Up) {
			if (!maze->cellSolid(nx, cy - 1) && x - nx <= distance) {
				x = nx;
				distance -= x - nx;
				dir = Direction::Up;
			}
		} else if (intent == Direction::Down) {
			if (!maze->cellSolid(nx, cy + 1) && x - nx <= distance) {
				x = nx;
				distance -= x - nx;
				dir = Direction::Down;
			}
		}
	}

	if (dir == Direction::Up) {
		y -= distance;
		if (maze->cellSolid(cx, cy - 1)) {
			y = qMax(y, (double)cy);
		}
	} else if (dir == Direction::Right) {
		x += distance;
		if (maze->cellSolid(cx + 1, cy)) {
			x = qMin(x, (double)cx);
		}
	} else if (dir == Direction::Down) {
		y += distance;
		if (maze->cellSolid(cx, cy + 1)) {
			y = qMin(y, (double)cy);
		}
	} else if (dir == Direction::Left) {
		x -= distance;
		if (maze->cellSolid(cx - 1, cy)) {
			x = qMax(x, (double)cx);
		}
	}
}

void Enemy::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setBrush(color);
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(0, 0, width(), width());
}