#pragma once

#include <QWidget>

#include "ScoreDisplay.h"

class TopBar : public QWidget {
	Q_OBJECT
public:
	TopBar(QWidget *parent = nullptr) : QWidget(parent) {
		// DRAW_STH_PLEASE("TopBar");
		auto layout = new QHBoxLayout(this);
		auto score = new ScoreDisplay("Score");
		layout->addWidget(score);
		auto high = new ScoreDisplay("Highscore");
		layout->addWidget(high);

		setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	}
};
