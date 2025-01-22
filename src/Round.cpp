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
	red->setHousePos(13.5, 11);
	entities.push_back(red);
	connect(red, &Enemy::positionChanged, this, [this, red](){ maze->repositionEntity(*red); });
	connect(this, &Round::phaseBroadcast, red, &Enemy::receivePhaseBroadcast);
	red->changePhase(0, EnemyPhase::FRIGHTENED);

	/*Enemy *random = new Enemy(QColor("#ababab"), new RandomBehaviour(), maze);
	entities.push_back(random);
	connect(random, &Enemy::positionChanged, this, [this, random](){ maze->repositionEntity(*random); });
	red->changePhase(0, EnemyPhase::CHASE);

	Enemy *scatter = new Enemy(QColor("#35ef35"), new ScatterBehaviour(20, 0), maze);
	entities.push_back(scatter);
	connect(scatter, &Enemy::positionChanged, this, [this, scatter](){ maze->repositionEntity(*scatter); });
	red->changePhase(0, EnemyPhase::CHASE);*/

	Enemy *ambush = new Enemy(QColor("#ffb9ff"), new AmbushBehaviour(), maze);
	ambush->setHousePos(13.5, 14);
	entities.push_back(ambush);
	connect(ambush, &Enemy::positionChanged, this, [this, ambush](){ maze->repositionEntity(*ambush); });
	connect(this, &Round::phaseBroadcast, ambush, &Enemy::receivePhaseBroadcast);
	ambush->changePhase(7.5, EnemyPhase::CHASE);

	Enemy *whimsical = new Enemy(QColor("#00ffff"), new WhimsicalBehaviour(red), maze);
	whimsical->setHousePos(12, 14);
	entities.push_back(whimsical);
	connect(whimsical, &Enemy::positionChanged, this, [this, whimsical](){ maze->repositionEntity(*whimsical); });
	connect(this, &Round::phaseBroadcast, whimsical, &Enemy::receivePhaseBroadcast);
	whimsical->changePhase(15, EnemyPhase::CHASE);

	Enemy *ignorant = new Enemy(QColor("#ffb951"), new IgnorantBehaviour(0, Maze::HEIGHT), maze);
	ignorant->setHousePos(15, 14);
	entities.push_back(ignorant);
	connect(ignorant, &Enemy::positionChanged, this, [this, ignorant](){ maze->repositionEntity(*ignorant); });
	connect(this, &Round::phaseBroadcast, ignorant, &Enemy::receivePhaseBroadcast);
	ignorant->changePhase(25, EnemyPhase::CHASE);

	connect(pinger, &QTimer::timeout, this, [this]() {
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
			maze->setCell(x, y, MazeCell::Empty);
			broadcastPhase(0, EnemyPhase::FRIGHTENED);
			broadcastPhase(8, EnemyPhase::CHASE);
			alternatePhase = EnemyPhase::SCATTER;
			double alternationTime = 8 + phaseAlternationTimer->remainingTime() / 1000.0;
			phaseAlternationTimer->start(alternationTime * 1000);
			emit pointsScored(50);
		}

		if (!maze->hasConsumables()) {
			emit roundEnded(true, maze);
		}
	});
	pinger->start(1000 / FPS);

	phaseBroadcastTimer->setSingleShot(true);
	connect(phaseBroadcastTimer, &QTimer::timeout, this, [this](){
		emit phaseBroadcast(nextPhase);
	});

	phaseAlternationTimer->setSingleShot(true);
	connect(phaseAlternationTimer, &QTimer::timeout, this, [this](){
		emit phaseBroadcast(alternatePhase);
		if (alternatePhase == EnemyPhase::SCATTER) {
			alternatePhase = EnemyPhase::CHASE;
			phaseAlternationTimer->start(7 * 1000);
		} else {
			alternatePhase = EnemyPhase::SCATTER;
			phaseAlternationTimer->start(20 * 1000);
		}
	});
	phaseAlternationTimer->start(0);
}

void Round::broadcastPhase(int delay, EnemyPhase phase) {
	if (delay == 0) {
		emit phaseBroadcast(phase);
		phaseBroadcastTimer->stop();
	} else {
		nextPhase = phase;
		phaseBroadcastTimer->start(delay * 1000);
	}
}