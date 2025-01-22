#pragma once

#include <vector>
#include <QWidget>
#include <QTimer>

#include "EnemyPhase.h"

class Maze;
class Entity;
class Player;

const double FPS = 120;

class Round : public QWidget {
    Q_OBJECT
public:
    explicit Round(Maze *oldMaze = nullptr, QWidget *parent = nullptr);
private slots:
    void broadcastPhase(int delay, EnemyPhase phase);
signals:
    void roundEnded(bool success, Maze *maze);
    void keyPressed(int key);
    void pointsScored(int points);
    void phaseBroadcast(EnemyPhase phase);
private:
    Maze *maze = nullptr;
	Player *player = nullptr;
	std::vector<Entity *> entities{};

    QTimer *pinger = new QTimer(this);

    QTimer *phaseBroadcastTimer = new QTimer(this);
    EnemyPhase nextPhase;

    QTimer *phaseAlternationTimer = new QTimer(this);
    EnemyPhase alternatePhase = EnemyPhase::SCATTER;
};