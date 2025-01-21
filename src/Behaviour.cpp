#include <QRandomGenerator>
#include <QtDebug>
#include <cmath>

#include "Game.h"
#include "Behaviour.h"
#include "Movable.h"
#include "Field.h"

Direction RandomBehaviour::getIntent(Game &game, Movable &m) {
	int x = std::round(m.x), y = std::round(m.y);
	bool up = !game.maze->cellSolid(x, y - 1) && m.dir != Direction::Down;
	bool right = !game.maze->cellSolid(x + 1, y) && m.dir != Direction::Left;
	bool down = !game.maze->cellSolid(x, y + 1) && m.dir != Direction::Up;
	bool left = !game.maze->cellSolid(x - 1, y) && m.dir != Direction::Right;
	int choices = up + right + down + left;
	int chosen = QRandomGenerator::global()->bounded(choices);
	// qDebug() << chosen << up << right << down << left;
	// qDebug() << (m.dir == Direction::Up) << (m.dir == Direction::Right) << (m.dir == Direction::Down) << (m.dir == Direction::Left);
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
	qWarning() << "RandomBehaviour did not choose a direction.";
	return Direction::Up;
}

Direction TargetedBehaviour::getIntent(Game &game, Movable &m, int targetX, int targetY) {
	auto field = PointField(*game.maze, targetX, targetY);
	int x = std::round(m.x), y = std::round(m.y);
	int min = INT_MAX;
	Direction mindir = Direction::Up;
	Direction cur = m.dir;
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

Direction ChaseBehaviour::getIntent(Game &game, Movable &m) {
	return TargetedBehaviour::getIntent(game, m, (int)std::round(game.player->x), (int)std::round(game.player->y));
}

Direction AmbushBehaviour::getIntent(Game &game, Movable &m) {
	int x = std::round(game.player->x);
	int y = std::round(game.player->y);
	switch (game.player->dir) {
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
	return TargetedBehaviour::getIntent(game, m, x, y);
}

Direction WhimsicalBehaviour::getIntent(Game &game, Movable &m) {
	int x = std::round(game.player->x);
	int y = std::round(game.player->y);
	int rx = std::round(reference->x);
	int ry = std::round(reference->y);
	int dx = x - rx;
	int dy = y - ry;
	return TargetedBehaviour::getIntent(game, m, x + dx, y + dy);
}

Direction ScatterBehaviour::getIntent(Game &game, Movable &m) {
	return TargetedBehaviour::getIntent(game, m, targetX, targetY);
}

Direction IgnorantBehaviour::getIntent(Game &game, Movable &m) {
	int d = game.player->x * game.player->x + m.x * m.y;
	if (d >= treshold * treshold) {
		return chase.getIntent(game, m);
	} else {
		return scatter.getIntent(game, m);
	}
}
