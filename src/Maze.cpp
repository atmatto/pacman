#include "Maze.h"
#include "Entity.h"

#include <QPen>
#include <QPainter>

Maze::Maze(const Maze *maze, QWidget *parent) : QWidget(parent) {
	setMinimumSize(300, 300);
	if (maze == nullptr) {
		resetCells();
	} else {
		for (int j = 0; j < HEIGHT; j++) {
			for (int i = 0; i < WIDTH; i++) {
				cells[j][i] = maze->cells[j][i];
			}
		}
	}
	emit entityRepositioningRequired();
}

void Maze::resizeEvent(QResizeEvent *event) {
	emit entityRepositioningRequired();
}

void Maze::repositionEntity(Entity &e) {
	const int w = width(), h = height();
	int dim = qMin(w, h);
	const int cw = qMin(dim / WIDTH, dim / HEIGHT);
	const int ch = cw;
	dim = cw * qMax(WIDTH, HEIGHT);
	const int tlX = w/2 - (cw * WIDTH)/2;
	const int tlY = h/2 - (ch * HEIGHT)/2;
	const int margin = cw/3;

	e.setGeometry(
		tlX + e.x * cw - cw/2 + margin,
		tlY + e.y * ch - ch/2 + margin,
		2 * (cw - margin),
		2 * (ch - margin)
	);
}

void Maze::paintEvent(QPaintEvent *event) {
	const int w = width(), h = height();
	int dim = qMin(w, h);
	const int cw = qMin(dim / WIDTH, dim / HEIGHT);
	const int ch = cw;
	dim = cw * qMax(WIDTH, HEIGHT);
	const int tlX = w/2 - (cw * WIDTH)/2;
	const int tlY = h/2 - (ch * HEIGHT)/2;
	const int dotUnit = qMax(cw / 6, 2);

	QPainter painter(this);

	painter.setBrush(Qt::black);
	painter.drawRect(tlX, tlY, cw * WIDTH - 1, ch * HEIGHT - 1);

	QColor wallColor("#2121ff");
	QPen wall(wallColor, 2);

	QColor dotColor("#fab9b0");
	QPen dot(dotColor);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (cells[y][x] == MazeCell::Wall) {
				painter.setBrush(Qt::black);
				painter.setPen(wall);
				if (cell(x, y - 1) == MazeCell::Wall && (
					!(cell(x - 1, y) == MazeCell::Wall && cell(x - 1, y - 1) == MazeCell::Wall)
					|| !(cell(x + 1, y) == MazeCell::Wall && cell(x + 1, y - 1) == MazeCell::Wall)
				)) {
					painter.drawLine(
						tlX + x * cw + cw/2, tlY + y * ch,
						tlX + x * cw + cw/2, tlY + y * ch + ch/2
					);
				}
				if (cell(x, y + 1) == MazeCell::Wall && (
					!(cell(x - 1, y) == MazeCell::Wall && cell(x - 1, y + 1) == MazeCell::Wall)
					|| !(cell(x + 1, y) == MazeCell::Wall && cell(x + 1, y + 1) == MazeCell::Wall)
				)) {
					painter.drawLine(
						tlX + x * cw + cw/2, tlY + y * ch + ch/2,
						tlX + x * cw + cw/2, tlY + y * ch + ch
					);
				}
				if (cell(x - 1, y) == MazeCell::Wall && (
					!(cell(x, y - 1) == MazeCell::Wall && cell(x - 1, y - 1) == MazeCell::Wall)
					|| !(cell(x, y + 1) == MazeCell::Wall && cell(x - 1, y + 1) == MazeCell::Wall)
				)) {
					painter.drawLine(
						tlX + x * cw, tlY + y * ch + ch/2,
						tlX + x * cw + cw/2, tlY + y * ch + ch/2
					);
				}
				if (cell(x + 1, y) == MazeCell::Wall && (
					!(cell(x, y - 1) == MazeCell::Wall && cell(x + 1, y - 1) == MazeCell::Wall)
					|| !(cell(x, y + 1) == MazeCell::Wall && cell(x + 1, y + 1) == MazeCell::Wall)
				)) {
					painter.drawLine(
						tlX + x * cw + cw/2, tlY + y * ch + ch/2,
						tlX + x * cw + cw, tlY + y * ch + ch/2
					);
				}
			} else if (cells[y][x] == MazeCell::Dot) {
				const int cx = tlX + x * cw + cw/2;
				const int cy = tlY + y * ch + ch/2;
				painter.setBrush(dotColor);
				painter.setPen(QPen());
				painter.drawRect(cx - dotUnit, cy - dotUnit, 2 * dotUnit, 2 * dotUnit);
			} else if (cells[y][x] == MazeCell::Energizer) {
				const int cx = tlX + x * cw + cw/2;
				const int cy = tlY + y * ch + ch/2;
				painter.setBrush(dotColor);
				painter.setPen(QPen());
				painter.drawEllipse(cx - (5 * dotUnit)/2, cy - (5 * dotUnit)/2, 5 * dotUnit, 5 * dotUnit);
			}
		}
	}
}

MazeCell Maze::cell(int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return MazeCell::Empty;
	}
	return cells[y][x];
}

void Maze::setCell(int x, int y, MazeCell cell) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return;
	}
	cells[y][x] = cell;
}

bool Maze::cellSolid(int x, int y) {
	MazeCell c = cell(x, y);
	return c == MazeCell::Wall;
}

bool Maze::hasConsumables() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (cells[y][x] == MazeCell::Dot || cells[y][x] == MazeCell::Energizer) {
				return true;
			}
		}
	}
	return false;
}