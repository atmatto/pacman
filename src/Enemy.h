#pragma once

#include <QColor>
#include <QTimer>

#include "Entity.h"

class Behaviour;
class Player;

class Enemy : public Entity {
public:
    Enemy(QColor color, Behaviour *behaviour, QWidget *parent = nullptr);
    void setHousePos(double x, double y);
    void setScatterPos(double x, double y);
    bool isFrightened();
public slots:
    void step(Maze &maze, double deltaTime, Player &player);
    void globalModeChanged(bool scatter);
    void beginFrightened();
    void endHoused();
    void consumed();
private slots:
    void endFrightened();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QColor color;
    QColor baseColor;

    bool scatter = false;

    bool housed = true;
    bool frightened = false;
    QTimer *frightenedTimer = new QTimer(this);
    unsigned long long int stepCounter = 0;

    double scatterX = -1; // TODO
    double scatterY = -1;
    double houseX = -1;
    double houseY = -1;

    Behaviour *behaviour;
    Behaviour *baseBehaviour;
};