#include "TopBar.h"
#include "ValueDisplay.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QStyle>

TopBar::TopBar(QWidget *parent) : QWidget(parent) {
    auto score = new ValueDisplay("Score");
    connect(this, &TopBar::updateScore, score, &ValueDisplay::update);
    auto high = new ValueDisplay("High score");
    connect(this, &TopBar::updateHighscore, high, &ValueDisplay::update);
	pause = new QPushButton(style()->standardIcon(QStyle::SP_MediaPause), "");
	connect(pause, &QPushButton::pressed, this, &TopBar::pauseToggled);
	exit = new QPushButton(style()->standardIcon(QStyle::SP_DialogCancelButton), "");
	connect(exit, &QPushButton::pressed, this, &TopBar::endGame);

    auto layout = new QHBoxLayout(this);
	layout->addWidget(pause);
	layout->addWidget(exit);
    layout->addWidget(score, 1);
    layout->addWidget(high, 1);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void TopBar::setPaused(bool paused) {
	pause->setIcon(style()->standardIcon(paused ? QStyle::SP_MediaPlay : QStyle::SP_MediaPause));
}