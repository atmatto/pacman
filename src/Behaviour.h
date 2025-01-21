#pragma once

class Game;
class Movable;
enum class Direction;

struct Behaviour {
	virtual Direction getIntent(Game &game, Movable &movable) = 0;
	virtual ~Behaviour() = default;
};

struct RandomBehaviour : public Behaviour {
	Direction getIntent(Game &game, Movable &movable) override;
};

struct TargetedBehaviour : public Behaviour {
	using Behaviour::getIntent;
	Direction getIntent(Game &game, Movable &movable, int targetX, int targetY);
};

struct ScatterBehaviour : public TargetedBehaviour {
	int targetX, targetY;
	ScatterBehaviour(int x, int y) : targetX(x), targetY(y) {}
	Direction getIntent(Game &game, Movable &movable) override;
};

struct ChaseBehaviour : public TargetedBehaviour {
	Direction getIntent(Game &game, Movable &movable) override;
};

struct AmbushBehaviour : public TargetedBehaviour {
	Direction getIntent(Game &game, Movable &movable) override;
};

struct WhimsicalBehaviour : public TargetedBehaviour {
	Movable *reference;
	WhimsicalBehaviour(Movable *reference) : reference(reference) {}
	Direction getIntent(Game &game, Movable &movable) override;
};

struct IgnorantBehaviour : public TargetedBehaviour {
	int targetX, targetY;
	int treshold = 8;

	IgnorantBehaviour(int x, int y) : targetX(x), targetY(y) {}

	ScatterBehaviour scatter = ScatterBehaviour(targetX, targetY);
	ChaseBehaviour chase = ChaseBehaviour();

	Direction getIntent(Game &game, Movable &movable) override;
};
