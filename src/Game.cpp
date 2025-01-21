#include "Game.h"
#include "Movable.h"

Game::Game() : QWidget() {
	auto layout = new QVBoxLayout(this);
	layout->addWidget(tbar);
	layout->addWidget(maze);
	layout->addWidget(bbar);

	Enemy *red = new Enemy(QColor("#ff0000"), maze);
	red->behaviour = new ChaseBehaviour();
	movables.push_back(red);

	Enemy *random = new Enemy(QColor("#ababab"), maze);
	random->behaviour = new RandomBehaviour();
	movables.push_back(random);

	Enemy *scatter = new Enemy(QColor("#35ef35"), maze);
	scatter->behaviour = new ScatterBehaviour(20, 0);
	movables.push_back(scatter);

	Enemy *ambush = new Enemy(QColor("#ffb8de"), maze);
	ambush->behaviour = new AmbushBehaviour();
	movables.push_back(ambush);

	Enemy *whimsical = new Enemy(QColor("#00ffde"), maze);
	whimsical->behaviour = new WhimsicalBehaviour(red);
	movables.push_back(whimsical);

	Enemy *ignorant = new Enemy(QColor("#ffb847"), maze);
	ignorant->behaviour = new IgnorantBehaviour(0, Maze::HEIGHT);
	movables.push_back(ignorant);
}

void Game::keyPressEvent(QKeyEvent *event) {
	// if (maze == nullptr || Player == nullptr) return; // TODO: remove
	switch (event->key()) {
		case Qt::Key_Up:
			player->intent = Direction::Up;
			break;
		case Qt::Key_Right:
			player->intent = Direction::Right;
			break;
		case Qt::Key_Down:
			player->intent = Direction::Down;
			break;
		case Qt::Key_Left:
			player->intent = Direction::Left;
			break;
	}
}

