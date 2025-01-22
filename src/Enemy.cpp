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

	frightenedTimer->setSingleShot(true);
	connect(frightenedTimer, &QTimer::timeout, this, &Enemy::endFrightened);
};

void Enemy::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	if (frightened) {
		painter.setBrush(QColor("#ffffff"));
	} else {
		painter.setBrush(color);
	}
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(0, 0, width(), width());
}

void Enemy::step(Maze &maze, double deltaTime, Player &player) {
	if (housed) {
		double dy = std::abs(0.6 * (2 * std::fmod(deltaTime * 120 * stepCounter + x * 4044851, 60) / 60 - 1));
		dy -= 0.3;
		x = houseX;
		y = houseY + dy;
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
	frightenedTimer->start(8 * 1000);
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
}

void Enemy::consumed() {
	housed = true;
	frightened = false;
	frightenedTimer->stop();
	QTimer::singleShot(3 * 1000, this, [this](){ endHoused(); });
	stepCounter = 0;
	if (behaviour != baseBehaviour) {
		delete behaviour;
	}
	behaviour = scatter ? new ScatterBehaviour(scatterX, scatterY) : baseBehaviour;
}

bool Enemy::isFrightened() {
	return frightened;
}