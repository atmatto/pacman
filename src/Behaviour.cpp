#include <QRandomGenerator>
#include <QtDebug>
#include <cmath>

#include "Behaviour.h"
#include "Entity.h"
#include "Maze.h"
#include "Player.h"
#include "DistanceField.h"

Direction RandomBehaviour::getIntent(Entity &e, Maze &maze, Player &player) {
	int x = std::round(e.x), y = std::round(e.y);
	bool up = !maze.cellSolid(x, y - 1) && e.dir != Direction::Down;
	bool right = !maze.cellSolid(x + 1, y) && e.dir != Direction::Left;
	bool down = !maze.cellSolid(x, y + 1) && e.dir != Direction::Up;
	bool left = !maze.cellSolid(x - 1, y) && e.dir != Direction::Right;
	int choices = up + right + down + left;
	int chosen = QRandomGenerator::global()->bounded(choices);
	if (up) {
		if (chosen == 0) return Direction::Up;
		chosen--;
	}
	if (right) {
		if (chosen == 0) return Direction::Right;
		chosen--;
	}
	if (down) {
		if (chosen == 0) return Direction::Down;
		chosen--;
	}
	if (left) {
		if (chosen == 0) return Direction::Left;
		chosen--;
	}
	return Direction::Up;
}

Direction TargetedBehaviour::getIntent(Entity &e, Maze &maze, Player &player, int targetX, int targetY) {
	auto field = DistanceField(maze, targetX, targetY);
	int x = std::round(e.x), y = std::round(e.y);
	int min = INT_MAX;
	Direction mindir = Direction::Up;
	Direction cur = e.dir;
	if (int d = field.get(x - 1, y); cur != Direction::Right && d < min) {
		min = d;
		mindir = Direction::Left;
	}
	if (int d = field.get(x + 1, y); cur != Direction::Left && d < min) {
		min = d;
		mindir = Direction::Right;
	}
	if (int d = field.get(x, y - 1); cur != Direction::Down && d < min) {
		min = d;
		mindir = Direction::Up;
	}
	if (int d = field.get(x, y + 1); cur != Direction::Up && d < min) {
		min = d;
		mindir = Direction::Down;
	}
	return mindir;
}

Direction ChaseBehaviour::getIntent(Entity &e, Maze &maze, Player &player) {
	return TargetedBehaviour::getIntent(e, maze, player, (int)std::round(player.x), (int)std::round(player.y));
}

Direction AmbushBehaviour::getIntent(Entity &e, Maze &maze, Player &player) {
	int x = std::round(player.x);
	int y = std::round(player.y);
	switch (player.dir) {
		case Direction::Up:
			y -= 4;
			break;
		case Direction::Right:
			x += 4;
			break;
		case Direction::Down:
			y += 4;
			break;
		case Direction::Left:
			x -= 4;
			break;
	}
	return TargetedBehaviour::getIntent(e, maze, player, x, y);
}

Direction WhimsicalBehaviour::getIntent(Entity &e, Maze &maze, Player &player) {
	int x = std::round(player.x);
	int y = std::round(player.y);
	int rx = std::round(reference->x);
	int ry = std::round(reference->y);
	int dx = x - rx;
	int dy = y - ry;
	return TargetedBehaviour::getIntent(e, maze, player, x + dx, y + dy);
}

Direction ScatterBehaviour::getIntent(Entity &e, Maze &maze, Player &player) {
	return TargetedBehaviour::getIntent(e, maze, player, targetX, targetY);
}

Direction IgnorantBehaviour::getIntent(Entity &e, Maze &maze, Player &player) {
	int d = (player.x - e.x) * (player.x - e.x) + (player.y - e.y) * (player.y - e.y);
	if (d >= treshold * treshold) {
		return chase.getIntent(e, maze, player);
	} else {
		return scatter.getIntent(e, maze, player);
	}
}