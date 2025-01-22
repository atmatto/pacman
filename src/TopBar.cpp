#include "TopBar.h"
#include "ValueDisplay.h"

#include <QHBoxLayout>

TopBar::TopBar(QWidget *parent) : QWidget(parent) {
    auto score = new ValueDisplay("Score");
    connect(this, &TopBar::updateScore, score, &ValueDisplay::update);
    auto high = new ValueDisplay("High score");
    connect(this, &TopBar::updateHighscore, high, &ValueDisplay::update);

    auto layout = new QHBoxLayout(this);
    layout->addWidget(score);
    layout->addWidget(high);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}