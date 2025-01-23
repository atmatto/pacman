#pragma once

#include <QWidget>

// Main menu.
class Menu : public QWidget {
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);
signals:
    void startGame();
};