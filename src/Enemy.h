#pragma once

#include <QColor>
#include <QTimer>

#include "Entity.h"
#include "EnemyPhase.h"

class Behaviour;
class Player;

class Enemy : public Entity {
public:
    Enemy(QColor color, Behaviour *behaviour, QWidget *parent = nullptr);
    void setHousePos(double x, double y);
public slots:
    void step(Maze &maze, double deltaTime, Player &player);
    void changePhase(double delay, EnemyPhase newPhase);
    void receivePhaseBroadcast(EnemyPhase newPhase);
private slots:
    void phaseChanged();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QColor color;
    QColor baseColor;

    Behaviour *behaviour;
    Behaviour *baseBehaviour;

    EnemyPhase phase = EnemyPhase::HOUSE;
    EnemyPhase nextPhase = EnemyPhase::HOUSE;
    QTimer *phaseTimer;
    unsigned long long int stepCounter = 0;

    double houseX = -1;
    double houseY = -1;
};