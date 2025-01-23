#pragma once

#include "Entity.h"

// Pac-Man, the player-controlled entity.
class Player : public Entity {
public:
    Player(QWidget *parent = nullptr);
public slots:
    void keyPressed(int key);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    int animationFrame = 0;
};