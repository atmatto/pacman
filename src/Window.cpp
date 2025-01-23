#include "Window.h"
#include "Menu.h"
#include "Game.h"
#include "Images.h"

#include <QApplication>
#include <QKeyEvent>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    init_images();

    Window window;
    window.show();

    return app.exec();
}

Window::Window(QWidget *parent) : QWidget(parent) {
    grabKeyboard();

    menu = new Menu();
    connect(menu, &Menu::startGame, this, &Window::startGame);

    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(menu);
}

void Window::startGame() {
    if (game != nullptr) {
        return;
    }

    menu->hide();

    game = new Game();
    connect(game, &Game::gameEnded, this, &Window::endGame);
    connect(game, &Game::pointsChanged, this, &Window::pointsChanged);
    connect(this, &Window::highscoreChanged, game, &Game::highscoreChanged);
	connect(this, &Window::keyPressed, game, &Game::keyPressed);
    emit highscoreChanged(highscore);

    layout->addWidget(game);
}

void Window::endGame() {
    game->hide();
    game->deleteLater();
    game = nullptr;
    menu->show();
}

void Window::pointsChanged(int points) {
    if (points > highscore) {
        highscore = points;
        emit highscoreChanged(highscore);
    }
}

void Window::keyPressEvent(QKeyEvent *event) {
	emit keyPressed(event->key());
}