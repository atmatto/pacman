#pragma once

#include <climits>

#include "Maze.h"

class DistanceField {
public:
	DistanceField(Maze &maze, int baseX, int baseY) : maze(&maze), baseX(baseX), baseY(baseY) {}
	int get(int x, int y) const {
		if (maze->cellSolid(x, y)) {
			return INT_MAX;
		} else {
			return (baseX - x) * (baseX - x) + (baseY - y) * (baseY - y);
		}
	}
private:
	Maze *maze;
	int baseX, baseY;
};
