#include "Menu.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

Menu::Menu(QWidget *parent) : QWidget(parent) {
    setStyleSheet(
        "QLabel { font-size: 32pt;}"
        "QPushButton { font-size: 24pt; padding: 20px; }"
    );

    auto label = new QLabel("Pacman");
    label->setAlignment(Qt::AlignCenter);
    auto button = new QPushButton("Play");
    connect(button, &QPushButton::pressed, this, &Menu::startGame);

    auto layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(label);
    layout->addSpacing(40);
    layout->addWidget(button);
    layout->addStretch();
    layout->setContentsMargins(120, 120, 120, 120);
}