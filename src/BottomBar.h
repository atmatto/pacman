#pragma once

#include <QWidget>

class ValueDisplay;

// In-game bottom bar showing the number of lives left.
class BottomBar : public QWidget {
	Q_OBJECT
public:
	explicit BottomBar(QWidget *parent = nullptr);
signals:
	void updateLives(int score);
};
