#pragma once

#include "Entity.h"

class Player : public Entity {
public:
    Player(QWidget *parent = nullptr);
public slots:
    void keyPressed(int key);
};