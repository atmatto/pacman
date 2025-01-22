#include "Round.h"
#include "Maze.h"
#include "Enemy.h"
#include "Player.h"
#include "Behaviour.h"

#include <QVBoxLayout>
#include <cmath>

Round::Round(Maze *oldMaze, QWidget *parent) : QWidget(parent) {
	if (oldMaze) {
		maze = new Maze(oldMaze);
	} else {
		maze = new Maze();
	}
	connect(maze, &Maze::entityRepositioningRequired, this, [this](){
		for (auto e : entities) {
			maze->repositionEntity(*e);
		}
	});

    auto layout = new QVBoxLayout(this);
    layout->addWidget(maze);

	player = new Player(maze);
	entities.push_back(player);
	connect(this, &Round::keyPressed, player, &Player::keyPressed);
	connect(player, &Player::positionChanged, this, [this](){ maze->repositionEntity(*player); });

	Enemy *red = new Enemy(QColor("#ff0000"), new ChaseBehaviour(), maze);
	red->setHousePos(13.5, 14);
	red->setScatterPos(Maze::WIDTH - 2, -2);
	entities.push_back(red);
	connect(red, &Enemy::positionChanged, this, [this, red](){ maze->repositionEntity(*red); });
	connect(this, &Round::energizerCollected, red, &Enemy::beginFrightened);
	connect(this, &Round::modeChanged, red, &Enemy::globalModeChanged);
	QTimer::singleShot(0 * 1000, red, [red](){ red->endHoused(); });

	Enemy *ambush = new Enemy(QColor("#ffb9ff"), new AmbushBehaviour(), maze);
	ambush->setHousePos(13.5, 14);
	ambush->setScatterPos(2, -2);
	entities.push_back(ambush);
	connect(ambush, &Enemy::positionChanged, this, [this, ambush](){ maze->repositionEntity(*ambush); });
	connect(this, &Round::energizerCollected, ambush, &Enemy::beginFrightened);
	connect(this, &Round::modeChanged, ambush, &Enemy::globalModeChanged);
	QTimer::singleShot(7 * 1000, ambush, [ambush](){ ambush->endHoused(); });

	Enemy *whimsical = new Enemy(QColor("#00ffff"), new WhimsicalBehaviour(red), maze);
	whimsical->setHousePos(12, 14);
	whimsical->setScatterPos(Maze::WIDTH, Maze::HEIGHT + 2);
	entities.push_back(whimsical);
	connect(whimsical, &Enemy::positionChanged, this, [this, whimsical](){ maze->repositionEntity(*whimsical); });
	connect(this, &Round::energizerCollected, whimsical, &Enemy::beginFrightened);
	connect(this, &Round::modeChanged, whimsical, &Enemy::globalModeChanged);
	QTimer::singleShot(15 * 1000, whimsical, [whimsical](){ whimsical->endHoused(); });

	Enemy *ignorant = new Enemy(QColor("#ffb951"), new IgnorantBehaviour(0, Maze::HEIGHT), maze);
	ignorant->setHousePos(15, 14);
	ignorant->setScatterPos(0, Maze::HEIGHT + 2);
	entities.push_back(ignorant);
	connect(ignorant, &Enemy::positionChanged, this, [this, ignorant](){ maze->repositionEntity(*ignorant); });
	connect(this, &Round::energizerCollected, ignorant, &Enemy::beginFrightened);
	connect(this, &Round::modeChanged, ignorant, &Enemy::globalModeChanged);
	QTimer::singleShot(25 * 1000, ignorant, [ignorant](){ ignorant->endHoused(); });

	connect(heartbeat, &QTimer::timeout, this, [this]() {
		for (auto e : entities) {
			if (auto *m = dynamic_cast<Enemy *>(e)) {
				emit m->step(*maze, 1/FPS, *player);
			} else {
				emit e->step(*maze, 1/FPS);
			}
		}

		int x = std::round(player->x), y = std::round(player->y);
		MazeCell cell = maze->cell(x, y);
		if (cell == MazeCell::Dot) {
			maze->setCell(x, y, MazeCell::Empty);
			emit pointsScored(10);
		} else if (cell == MazeCell::Energizer) {
			consumptionBonus = 100;
			maze->setCell(x, y, MazeCell::Empty);
			scatter = false;
			emit modeChanged(scatter);
			modeTimer->start(20 * 1000);
			emit energizerCollected();
			emit pointsScored(50);
		}

		if (!maze->hasConsumables()) {
			emit roundEnded(true, maze);
		}

		for (auto e : entities) {
			if (auto *m = dynamic_cast<Enemy *>(e)) {
				if (std::abs(m->x - x) < 0.7 && std::abs(m->y - y) < 0.7) {
					if (m->isFrightened()) {
						m->consumed();
						emit pointsScored(consumptionBonus);
						consumptionBonus += 100;
					} else {
						emit roundEnded(false, maze);
					}
				}
			}
		}
	});
	heartbeat->start(1000 / FPS);

	modeTimer->setSingleShot(true);
	connect(modeTimer, &QTimer::timeout, this, [this](){
		scatter = !scatter;
		if (scatter) {
			modeTimer->start(7 * 1000);
		} else {
			modeTimer->start(20 * 1000);
		}
		emit modeChanged(scatter);
	});
	modeTimer->start(0);
}