#pragma once

#include <QColor>

#include "Timer.h"
#include "Entity.h"

class Behaviour;
class Player;

class Enemy : public Entity {
public:
    Enemy(double housedTime, QImage *image, Behaviour *behaviour, QWidget *parent = nullptr);
    void setHousePos(double x, double y);
    void setScatterPos(double x, double y);
    bool isFrightened();
public slots:
    void step(Maze &maze, double deltaTime, Player &player);
    void globalModeChanged(bool scatter);
    void beginFrightened();
    void endHoused();
    void consumed(); // by the player
    void setPaused(bool paused);
private slots:
    void endFrightened();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QImage *image;

    bool scatter = false;

    bool housed = true;
    Timer *housedTimer = new Timer(true, this);
    bool frightened = false;
    Timer *frightenedTimer = new Timer(true, this);
    unsigned long long int stepCounter = 0;

    double scatterX = -1;
    double scatterY = -1;
    double houseX = -1;
    double houseY = -1;

    // How soon to start various transition animations:
    const double dehousingTime = 1.5;
    const double blinkingTime = 2;

    Behaviour *behaviour;
    Behaviour *baseBehaviour;
};