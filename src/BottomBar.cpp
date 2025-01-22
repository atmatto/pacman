#include "BottomBar.h"
#include "ValueDisplay.h"

#include <QHBoxLayout>

BottomBar::BottomBar(QWidget *parent) : QWidget(parent) {
    auto lives = new ValueDisplay("Lives");
    connect(this, &BottomBar::updateLives, lives, &ValueDisplay::update);

    auto layout = new QHBoxLayout(this);
    layout->addWidget(lives);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}