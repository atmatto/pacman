#pragma once

#include <QWidget>
#include <QVBoxLayout>

class Menu;
class Game;

class Window : public QWidget {
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
private slots:
    void startGame();
    void endGame();
    void pointsChanged(int points);
signals:
    void highscoreChanged(int highscore);
	void keyPressed(int key);
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    QVBoxLayout *layout = new QVBoxLayout(this);
    Menu *menu = nullptr;
    Game *game = nullptr;

    int highscore = 0;
};