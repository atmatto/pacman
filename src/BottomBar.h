#pragma once

#include <QWidget>

class ValueDisplay;

class BottomBar : public QWidget {
	Q_OBJECT
public:
	explicit BottomBar(QWidget *parent = nullptr);
signals:
	void updateLives(int score);
};
