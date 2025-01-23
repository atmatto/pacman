#include "Menu.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

Menu::Menu(QWidget *parent) : QWidget(parent) {
    setStyleSheet(
        "QLabel { font-size: 26pt;}"
        "QPushButton { font-size: 22pt; padding: 20px; }"
    );

	auto drawing = new QLabel();
    drawing->setPixmap(QPixmap(":/drawing.png"));
    drawing->setAlignment(Qt::AlignCenter);
    auto label = new QLabel("Pac-Man");
    label->setAlignment(Qt::AlignCenter);
    auto button = new QPushButton("Play");
    connect(button, &QPushButton::pressed, this, &Menu::startGame);

    auto layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(drawing);
    layout->addSpacing(40);
    layout->addWidget(label);
    layout->addSpacing(40);
    layout->addWidget(button);
    layout->addStretch();
    layout->setContentsMargins(120, 120, 120, 120);
}