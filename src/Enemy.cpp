#include "Enemy.h"
#include "Behaviour.h"
#include "Images.h"

#include <QPainter>
#include <QRect>
#include <QImage>
#include <cmath>

Enemy::Enemy(double housedTime, QImage *image, Behaviour *behaviour, QWidget *parent) : Entity(parent) {
	this->image = image;
	this->behaviour = baseBehaviour = behaviour;

	speed = baseSpeed = 7;

	connect(housedTimer, &Timer::timeout, this, [this](){ endHoused(); });
	housedTimer->start(housedTime);
	connect(frightenedTimer, &Timer::timeout, this, &Enemy::endFrightened);
};

void Enemy::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	if (frightened) {
		double blinkingProgress = (blinkingTime - frightenedTimer->remaining()) / blinkingTime;
		if (blinkingProgress >= 0 && fmod(blinkingProgress, 0.25) < 0.25 / 2) {
			painter.drawImage(QRect(0, 0, width(), height()), GHOST_BLINK);
		} else {
			painter.drawImage(QRect(0, 0, width(), height()), GHOST_FRIGHTENED);
		}
	} else {
		painter.drawImage(QRect(0, 0, width(), height()), *image);
	}
}

void Enemy::step(Maze &maze, double deltaTime, Player &player) {
	if (housed) {
		double dy = std::abs(0.6 * (2 * std::fmod(deltaTime * 120 * stepCounter + x * 4044851, 60) / 60 - 1));
		dy -= 0.3;
		if (stepCounter <= 1) {
			dy = 0;
		}
		x = houseX;
		y = houseY + dy;
		double dehousingProgress = 2 * (dehousingTime - housedTimer->remaining()) / dehousingTime;
		if (dehousingProgress > 1) {
			dehousingProgress--;
			x = 13.5;
			y = houseY + (11 - houseY) * dehousingProgress;
		} else if (dehousingProgress > 0) {
			x = houseX + (13.5 - houseX) * dehousingProgress;
			y = houseY;
		}
		emit positionChanged();
	} else {
		if (stepCounter == 0) {
			if (intent == Direction::Left) intent = Direction::Right;
			if (intent == Direction::Right) intent = Direction::Left;
			if (intent == Direction::Up) intent = Direction::Down;
			if (intent == Direction::Down) intent = Direction::Up;
		} else {
			intent = behaviour->getIntent(*this, maze, player);
		}
		Entity::step(maze, deltaTime);
	}
	stepCounter++;
}

void Enemy::globalModeChanged(bool scatter) {
	this->scatter = scatter;
	if (behaviour != baseBehaviour) {
		delete behaviour;
	}
	if (!scatter) {
		behaviour = baseBehaviour;
	} else {
		behaviour = new ScatterBehaviour(scatterX, scatterY);
	}
	stepCounter = 0;
}

void Enemy::setHousePos(double x, double y) {
	houseX = x;
	houseY = y;
}

void Enemy::setScatterPos(double x, double y) {
	scatterX = x;
	scatterY = y;
}

void Enemy::beginFrightened() {
	if (!frightened) {
		frightened = true;
		if (behaviour != baseBehaviour) {
			delete behaviour;
		}
		behaviour = new RandomBehaviour();
	}
	frightenedTimer->start(8);
	speed = baseSpeed / 2;
}

void Enemy::endHoused() {
	housed = false;
	x = 13.5;
	y = 11;
	dir = Direction::Left;
	intent = Direction::Left;
}

void Enemy::endFrightened() {
	if (behaviour != baseBehaviour) {
		delete behaviour;
		behaviour = baseBehaviour;
	}
	globalModeChanged(scatter);
	frightened = false;
	speed = baseSpeed;
}

void Enemy::consumed() {
	housed = true;
	frightened = false;
	speed = baseSpeed;
	frightenedTimer->stop();
	housedTimer->start(3);
	stepCounter = 0;
	if (behaviour != baseBehaviour) {
		delete behaviour;
	}
	behaviour = scatter ? new ScatterBehaviour(scatterX, scatterY) : baseBehaviour;
}

bool Enemy::isFrightened() {
	return frightened;
}

void Enemy::setPaused(bool paused) {
	if (paused) {
		housedTimer->pause();
		frightenedTimer->pause();
	} else {
		housedTimer->resume();
		frightenedTimer->resume();
	}
}