#pragma once

#include <QWidget>

class ValueDisplay;

class TopBar : public QWidget {
	Q_OBJECT
public:
	explicit TopBar(QWidget *parent = nullptr);
signals:
	void updateScore(int score);
	void updateHighscore(int highscore);
private:
	//ValueDisplay *score;
	//ValueDisplay *high;
};
