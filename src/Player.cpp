#include "Player.h"

#include <QDebug>

Player::Player(QWidget *parent) : Entity(parent) {
	x = 13.5;
	y = 23;
};

void Player::keyPressed(int key) {
	switch (key) {
		case Qt::Key_Up:
			intent = Direction::Up;
			break;
		case Qt::Key_Right:
			intent = Direction::Right;
			break;
		case Qt::Key_Down:
			intent = Direction::Down;
			break;
		case Qt::Key_Left:
			intent = Direction::Left;
			break;
	}
}