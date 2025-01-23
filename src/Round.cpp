#include "Round.h"
#include "Maze.h"
#include "Enemy.h"
#include "Player.h"
#include "Behaviour.h"
#include "Images.h"

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

	connect(this, &Round::keyPressed, this, &Round::pressKey);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(maze);

	player = new Player(maze);
	entities.push_back(player);
	connect(this, &Round::keyPressed, player, &Player::keyPressed);
	connect(player, &Player::positionChanged, this, [this](){ maze->repositionEntity(*player); });

	Enemy *red = new Enemy(0, &GHOST_RED, new ChaseBehaviour(), maze);
	red->setHousePos(13.4, 14);
	red->setScatterPos(Maze::WIDTH - 2, -2);
	entities.push_back(red);
	connect(red, &Enemy::positionChanged, this, [this, red](){ maze->repositionEntity(*red); });
	connect(this, &Round::energizerCollected, red, &Enemy::beginFrightened);
	connect(this, &Round::modeChanged, red, &Enemy::globalModeChanged);
	connect(this, &Round::pauseChanged, red, &Enemy::setPaused);

	Enemy *ambush = new Enemy(7, &GHOST_AMBUSH, new AmbushBehaviour(), maze);
	ambush->setHousePos(13.5, 14);
	ambush->setScatterPos(2, -2);
	entities.push_back(ambush);
	connect(ambush, &Enemy::positionChanged, this, [this, ambush](){ maze->repositionEntity(*ambush); });
	connect(this, &Round::energizerCollected, ambush, &Enemy::beginFrightened);
	connect(this, &Round::modeChanged, ambush, &Enemy::globalModeChanged);
	connect(this, &Round::pauseChanged, ambush, &Enemy::setPaused);

	Enemy *whimsical = new Enemy(15, &GHOST_WHIMSICAL, new WhimsicalBehaviour(red), maze);
	whimsical->setHousePos(12, 14);
	whimsical->setScatterPos(Maze::WIDTH, Maze::HEIGHT + 2);
	entities.push_back(whimsical);
	connect(whimsical, &Enemy::positionChanged, this, [this, whimsical](){ maze->repositionEntity(*whimsical); });
	connect(this, &Round::energizerCollected, whimsical, &Enemy::beginFrightened);
	connect(this, &Round::modeChanged, whimsical, &Enemy::globalModeChanged);
	connect(this, &Round::pauseChanged, whimsical, &Enemy::setPaused);

	Enemy *ignorant = new Enemy(25, &GHOST_IGNORANT, new IgnorantBehaviour(0, Maze::HEIGHT), maze);
	ignorant->setHousePos(15, 14);
	ignorant->setScatterPos(0, Maze::HEIGHT + 2);
	entities.push_back(ignorant);
	connect(ignorant, &Enemy::positionChanged, this, [this, ignorant](){ maze->repositionEntity(*ignorant); });
	connect(this, &Round::energizerCollected, ignorant, &Enemy::beginFrightened);
	connect(this, &Round::modeChanged, ignorant, &Enemy::globalModeChanged);
	connect(this, &Round::pauseChanged, ignorant, &Enemy::setPaused);

	connect(heartbeat, &Timer::timeout, this, [this]() {
		stepEntities();
		eatDots();
		checkWinningCondition();
		entityInteractions();
	});
	heartbeat->start(1 / FPS);
	emit heartbeat->timeout();

	connect(modeTimer, &Timer::timeout, this, [this]() {
		scatter = !scatter;
		if (scatter) {
			modeTimer->start(7);
		} else {
			modeTimer->start(20);
		}
		emit modeChanged(scatter);
	});
	modeTimer->start(0);

	setPaused(true);
	Timer::quick(this, 1, [this]() {
		setPaused(false);
	});
}

void Round::setPaused(bool paused) {
	if (paused) {
		heartbeat->pause();
		modeTimer->pause();
	} else {
		heartbeat->resume();
		modeTimer->resume();
	}
	this->paused = paused;
	emit pauseChanged(paused);
}

void Round::togglePaused() {
	setPaused(!paused);
}

void Round::pressKey(int key) {
	if (key == Qt::Key_Escape) {
		setPaused(!paused);
	}
}

void Round::stepEntities() {
	for (auto e : entities) {
		if (auto *m = dynamic_cast<Enemy *>(e)) {
			emit m->step(*maze, 1/FPS, *player);
		} else {
			emit e->step(*maze, 1/FPS);
		}
	}
}

void Round::eatDots() {
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
		modeTimer->start(20);
		emit energizerCollected();
		emit pointsScored(50);
	}
}

void Round::checkWinningCondition() {
	if (!maze->hasConsumables()) {
		setPaused(true);
		heartbeat->stop();
		Timer::quick(this, 2, [this]() {
			emit roundEnded(true, maze);
		});
	}
}

void Round::entityInteractions() {
	int x = std::round(player->x), y = std::round(player->y);
	for (auto e : entities) {
		if (auto *m = dynamic_cast<Enemy *>(e)) {
			if (std::abs(m->x - x) < 0.7 && std::abs(m->y - y) < 0.7) {
				if (m->isFrightened()) {
					m->consumed();
					emit pointsScored(consumptionBonus);
					consumptionBonus += 100;
				} else {
					setPaused(true);
					heartbeat->stop();
					Timer::quick(this, 2, [this]() {
						emit roundEnded(false, maze);
					});
				}
			}
		}
	}
}