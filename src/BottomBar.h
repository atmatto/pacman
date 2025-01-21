#pragma once

#include <QWidget>
#include <QHBoxLayout>

#include "LivesDisplay.h"

class BottomBar : public QWidget {
	Q_OBJECT
public:
	BottomBar(QWidget *parent = nullptr) : QWidget(parent) {
		auto layout = new QHBoxLayout(this);
		auto lives = new LivesDisplay();
		layout->addWidget(lives);

		setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	}
};
