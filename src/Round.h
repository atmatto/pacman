#pragma once

#include <vector>
#include <QWidget>
#include <QTimer>

class Maze;
class Entity;
class Player;

const double FPS = 60;

class Round : public QWidget {
    Q_OBJECT
public:
    explicit Round(Maze *oldMaze = nullptr, QWidget *parent = nullptr);
signals:
    void roundEnded(bool success, Maze *maze);
    void keyPressed(int key);
    void pointsScored(int points);
    void modeChanged(bool scatter);
    void energizerCollected();
private:
    Maze *maze = nullptr;
	Player *player = nullptr;
	std::vector<Entity *> entities{};

    QTimer *heartbeat = new QTimer(this);

    bool scatter = false;
    QTimer *modeTimer = new QTimer(this);

    int consumptionBonus = 100;
};