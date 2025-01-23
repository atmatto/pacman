#pragma once

#include <climits>

#include "Maze.h"

// DistanceField is a calculator of the distance between
// the base point and the coordinates supplied to get().
class DistanceField {
public:
	DistanceField(Maze &maze, int baseX, int baseY) : maze(&maze), baseX(baseX), baseY(baseY) {}

	// Returns the distance to the base point
	// or INT_MAX if the queried cell is solid.
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
