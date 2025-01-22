#include "Enemy.h"
#include "Behaviour.h"

#include <QPainter>
#include <QTimer>
#include <cmath>

Enemy::Enemy(QColor color, Behaviour *behaviour, QWidget *parent) : Entity(parent) {
	this->color = baseColor = color;
	this->behaviour = baseBehaviour = behaviour;

	speed = 7;
	x = y = 11;

	phaseTimer = new QTimer(this);
	phaseTimer->setSingleShot(true);
	connect(phaseTimer, &QTimer::timeout, this, &Enemy::phaseChanged);
	phaseChanged();
};

void Enemy::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	if (phase == EnemyPhase::FRIGHTENED || phase == EnemyPhase::HOUSE_FRIGHTENED) {
		painter.setBrush(QColor("#ffffff"));
	} else {
		painter.setBrush(color);
	}
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(0, 0, width(), width());
}

void Enemy::step(Maze &maze, double deltaTime, Player &player) {
	if (phase == EnemyPhase::HOUSE || phase == EnemyPhase::HOUSE_FRIGHTENED) {
		double dy = std::abs(0.6 * (2 * std::fmod(deltaTime * 120 * stepCounter + x * 4044851, 60) / 60 - 1));
		dy -= 0.3;
		x = houseX;
		y = houseY + dy;
		emit positionChanged();
	} else {
		intent = behaviour->getIntent(*this, maze, player);
		Entity::step(maze, deltaTime);
	}
	stepCounter++;
}

void Enemy::phaseChanged() {
	if ((phase == EnemyPhase::HOUSE
			&& nextPhase != EnemyPhase::HOUSE
			&& nextPhase != EnemyPhase::HOUSE_FRIGHTENED)
		|| (phase == EnemyPhase::HOUSE_FRIGHTENED
			&& nextPhase != EnemyPhase::HOUSE
			&& nextPhase != EnemyPhase::HOUSE_FRIGHTENED)
	) {
		x = 13.5;
		y = 11;
	}
	if (nextPhase == EnemyPhase::FRIGHTENED) {
		if (behaviour != baseBehaviour) {
			delete behaviour;
		}
		behaviour = new RandomBehaviour();
	} else if (nextPhase == EnemyPhase::CHASE) {
		if (behaviour != baseBehaviour) {
			delete behaviour;
		}
		behaviour = baseBehaviour;
	} else if (nextPhase == EnemyPhase::SCATTER) {
		if (behaviour != baseBehaviour) {
			delete behaviour;
		}
		behaviour = new ScatterBehaviour(0, 0); // TODO
	}
	stepCounter = 0;
	phase = nextPhase;
}

void Enemy::changePhase(double delay, EnemyPhase newPhase) {
	nextPhase = newPhase;
	if (delay == 0) {
		phaseChanged();
		phaseTimer->stop();
	} else {
		phaseTimer->start(delay * 1000);
	}
}

void Enemy::setHousePos(double x, double y) {
	houseX = x;
	houseY = y;
}

void Enemy::receivePhaseBroadcast(EnemyPhase newPhase) {
	if (newPhase == EnemyPhase::SCATTER || newPhase == EnemyPhase::CHASE) {
		if (phase == EnemyPhase::HOUSE || phase == EnemyPhase::HOUSE_FRIGHTENED) {
			changePhase(0, EnemyPhase::HOUSE);
			changePhase(phaseTimer->remainingTime() / 1000.0, newPhase);
		} else {
			changePhase(0, newPhase);
		}
	} else if (newPhase == EnemyPhase::FRIGHTENED) {
		if (phase == EnemyPhase::HOUSE || phase == EnemyPhase::HOUSE_FRIGHTENED) {
			changePhase(0, EnemyPhase::HOUSE_FRIGHTENED);
			changePhase(phaseTimer->remainingTime() / 1000.0, newPhase);
		} else {
			changePhase(0, newPhase);
		}
	}
}