#pragma once

#include "Direction.h"

class Entity;
class Maze;
class Player;

struct Behaviour {
	virtual Direction getIntent(Entity &entity, Maze &maze, Player &player) = 0;
	virtual ~Behaviour() = default;
};

struct RandomBehaviour : public Behaviour {
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

struct TargetedBehaviour : public Behaviour {
	using Behaviour::getIntent;
	Direction getIntent(Entity &entity, Maze &maze, Player &player, int targetX, int targetY);
};

struct ScatterBehaviour : public TargetedBehaviour {
	int targetX, targetY;
	ScatterBehaviour(int x, int y) : targetX(x), targetY(y) {}
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

struct ChaseBehaviour : public TargetedBehaviour {
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

struct AmbushBehaviour : public TargetedBehaviour {
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

struct WhimsicalBehaviour : public TargetedBehaviour {
	Entity *reference;
	WhimsicalBehaviour(Entity *reference) : reference(reference) {}
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

struct IgnorantBehaviour : public TargetedBehaviour {
	int targetX, targetY;
	int treshold = 8;

	IgnorantBehaviour(int x, int y) : targetX(x), targetY(y) {}

	ScatterBehaviour scatter = ScatterBehaviour(targetX, targetY);
	ChaseBehaviour chase = ChaseBehaviour();

	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};