#pragma once

#include "Direction.h"

class Entity;
class Maze;
class Player;

// Behaviour decides what direction an enemy will take.
struct Behaviour {
	virtual Direction getIntent(Entity &entity, Maze &maze, Player &player) = 0;
	virtual ~Behaviour() = default;
};

// Choose the direction randomly.
struct RandomBehaviour : public Behaviour {
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

// Go towards the target which may change at any time.
struct TargetedBehaviour : public Behaviour {
	using Behaviour::getIntent;
	Direction getIntent(Entity &entity, Maze &maze, Player &player, int targetX, int targetY);
};

// Go towards a fixed target.
struct ScatterBehaviour : public TargetedBehaviour {
	int targetX, targetY;
	ScatterBehaviour(int x, int y) : targetX(x), targetY(y) {}
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

// Go towards the player.
struct ChaseBehaviour : public TargetedBehaviour {
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

// Go towards the cell 4 units in front of the player.
struct AmbushBehaviour : public TargetedBehaviour {
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

// Go towards the target chosen by doubling the length
// of the vector from a reference enemy to the player.
struct WhimsicalBehaviour : public TargetedBehaviour {
	Entity *reference;
	WhimsicalBehaviour(Entity *reference) : reference(reference) {}
	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};

// Chase the player but retreat to the fixed target when
// the distance to the player is lower than the treshold.
struct IgnorantBehaviour : public TargetedBehaviour {
	int targetX, targetY;
	int treshold = 8;

	IgnorantBehaviour(int x, int y) : targetX(x), targetY(y) {}

	ScatterBehaviour scatter = ScatterBehaviour(targetX, targetY);
	ChaseBehaviour chase = ChaseBehaviour();

	Direction getIntent(Entity &entity, Maze &maze, Player &player) override;
};