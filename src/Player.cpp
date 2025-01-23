#include "Player.h"
#include "Images.h"

#include <QPainter>

void Player::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	animationFrame = (animationFrame + 1) % PACMAN.size();
	painter.translate(width()/2, height()/2);
	if (dir == Direction::Down) {
		painter.rotate(90);
	} else if (dir == Direction::Up) {
		painter.rotate(-90);
	} else if (dir == Direction::Left) {
		painter.rotate(180);
	}
	painter.translate(-width()/2, -height()/2);
	painter.drawImage(QRect(0, 0, width(), height()), PACMAN[animationFrame]);
}

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