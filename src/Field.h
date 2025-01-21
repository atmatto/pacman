#pragma once

#include <queue>
#include <climits>

#include "Maze.h"

class BaseField {
public:
	int cells[Maze::HEIGHT][Maze::WIDTH] = {};
	Maze *maze;

	BaseField(Maze &maze) : maze(&maze) {}
	virtual ~BaseField() = default;

	int get(int x, int y) const {
		if (x < 0 || y < 0 || x >= Maze::WIDTH || y >= Maze::HEIGHT) {
			return INT_MAX;
		}
		return cells[y][x];
	}
};

class PointField : public BaseField {
public:
	const int x, y;

	PointField(Maze &maze, int x, int y) : BaseField(maze), x(x), y(y) {
		x = qMin(qMax(x, 0), Maze::WIDTH - 1);
		y = qMin(qMax(y, 0), Maze::HEIGHT - 1);

		for (int x = 0; x < Maze::WIDTH; x++) {
			for (int y = 0; y < Maze::HEIGHT; y++) {
				cells[y][x] = INT_MAX;
			}
		}

		cells[y][x] = 0;

		std::queue<std::pair<int, int>> q{};
		q.push({x, y});
		while (!q.empty()) {
			auto point = q.front();
			q.pop();
			int x = point.first, y = point.second;
			if (x - 1 >= 0 && !maze.cellSolid(x - 1, y) && cells[y][x - 1] > cells[y][x] + 1) {
				cells[y][x - 1] = cells[y][x] + 1;
				q.push({x - 1, y});
			}
			if (x + 1 < Maze::WIDTH && !maze.cellSolid(x + 1, y) && cells[y][x + 1] > cells[y][x] + 1) {
				cells[y][x + 1] = cells[y][x] + 1;
				q.push({x + 1, y});
			}
			if (y - 1 >= 0 && !maze.cellSolid(x, y - 1) && cells[y - 1][x] > cells[y][x] + 1) {
				cells[y - 1][x] = cells[y][x] + 1;
				q.push({x, y - 1});
			}
			if (y + 1 < Maze::HEIGHT && !maze.cellSolid(x, y + 1) && cells[y + 1][x] > cells[y][x] + 1) {
				cells[y + 1][x] = cells[y][x] + 1;
				q.push({x, y + 1});
			}
		}
	}
};
