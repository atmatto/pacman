#pragma once

#include <QWidget>
#include <QVBoxLayout>

class TopBar;
class BottomBar;
class Round;
class Maze;

// A single game which lasts until all lives are exhausted.
// Consists of rounds. As a widget, Game serves as the wrapper
// for top and bottom bars and the maze (Round widget).
class Game : public QWidget {
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
signals:
    void gameEnded();
    void pointsChanged(int points);
    void livesChanged(int lives);
    void highscoreChanged(int highscore);
    void keyPressed(int key);
    void togglePaused();
    void setPaused(bool paused);
private slots:
    void addPoints(int points);
    void beginRound(Maze *oldMaze);
    void endRound(bool success, Maze *maze);
private:
    int points = 0;
    int lives = 2;

    QVBoxLayout *layout;

    TopBar *top = nullptr;
    BottomBar *bottom = nullptr;

    Round *round = nullptr;
};