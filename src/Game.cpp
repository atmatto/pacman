#include "Game.h"
#include "TopBar.h"
#include "BottomBar.h"
#include "Round.h"
#include "Maze.h"

Game::Game(QWidget *parent) : QWidget(parent) {
    top = new TopBar();
    connect(this, &Game::pointsChanged, top, &TopBar::updateScore);
    connect(this, &Game::highscoreChanged, top, &TopBar::updateHighscore);
    connect(this, &Game::setPaused, top, &TopBar::setPaused);
    connect(top, &TopBar::pauseToggled, this, &Game::togglePaused);
    connect(top, &TopBar::endGame, this, &Game::gameEnded);
    bottom = new BottomBar();
    connect(this, &Game::livesChanged, bottom, &BottomBar::updateLives);
    emit livesChanged(lives);

    layout = new QVBoxLayout(this);
    layout->addWidget(top);
    layout->addWidget(bottom);

    beginRound(nullptr);
}

void Game::addPoints(int points) {
    this->points += points;
    emit pointsChanged(this->points);
    top->updateScore(this->points);
}

void Game::beginRound(Maze *oldMaze) {
    if (round != nullptr) {
        round->deleteLater();
    }

    round = new Round(oldMaze);
    connect(round, &Round::roundEnded, this, &Game::endRound);
    connect(round, &Round::pointsScored, this, &Game::addPoints);
    connect(this, &Game::keyPressed, round, &Round::keyPressed);
    connect(round, &Round::pauseChanged, this, &Game::setPaused);
    connect(this, &Game::togglePaused, round, &Round::togglePaused);

    layout->insertWidget(1, round);
}

void Game::endRound(bool success, Maze *maze) {
    if (!success) {
        lives--;
    } else {
        maze = nullptr;
    }
    if (lives == -1) {
        emit gameEnded();
    } else {
        emit livesChanged(lives);
        beginRound(maze);
    }
}

