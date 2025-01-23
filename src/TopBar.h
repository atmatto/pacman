#pragma once

#include <QWidget>
#include <QPushButton>

class ValueDisplay;

// Top bar containing control buttons and score information.
class TopBar : public QWidget {
	Q_OBJECT
public:
	explicit TopBar(QWidget *parent = nullptr);
public slots:
	void setPaused(bool paused);
signals:
	void updateScore(int score);
	void updateHighscore(int highscore);
	void pauseToggled();
	void endGame();
private:
	QPushButton *pause;
	QPushButton *exit;
};
