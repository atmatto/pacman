#pragma once

#include <vector>
#include <QWidget>

#include "Timer.h"

class Maze;
class Entity;
class Player;

const double FPS = 60;

// The period of game from Pac-Man's deployment to his death.
// As a widget, Round serves as the wrapper of the maze.
class Round : public QWidget {
    Q_OBJECT
public:
    explicit Round(Maze *oldMaze = nullptr, QWidget *parent = nullptr);
public slots:
    void setPaused(bool paused);
    void togglePaused();
signals:
    // The maze state might need to be carried to the next round.
    void roundEnded(bool success, Maze *maze);
    void keyPressed(int key);
    void pointsScored(int points);
    void modeChanged(bool scatter);
    void energizerCollected();
    void pauseChanged(bool paused);
private slots:
    void pressKey(int key);
private:
    void stepEntities();
    void eatDots();
    void checkWinningCondition();
    void entityInteractions();

    Maze *maze = nullptr;
	Player *player = nullptr;
	std::vector<Entity *> entities{};

    // Main timer activating actions which need to happen every frame.
    Timer *heartbeat = new Timer(false, this);

    // Regular alternation between chase and scatter modes:
    bool scatter = false;
    Timer *modeTimer = new Timer(true, this);

    // When ghosts are in the frightened mode, each eaten ghost
    // increases the reward for that activity by 100.
    int consumptionBonus = 100;

    bool paused = false;
};