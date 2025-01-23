#include "Images.h"

#include <QColor>

QImage GHOST_RED;
QImage GHOST_AMBUSH;
QImage GHOST_WHIMSICAL;
QImage GHOST_IGNORANT;
QImage GHOST_FRIGHTENED;
QImage GHOST_BLINK;
std::vector<QImage> PACMAN{};

void init_images() {
	PACMAN = {
		QImage(":/pacman1.png"),
		QImage(":/pacman2.png"),
		QImage(":/pacman3.png"),
		QImage(":/pacman4.png"),
		QImage(":/pacman5.png"),
		QImage(":/pacman6.png"),
		QImage(":/pacman7.png"),
		QImage(":/pacman8.png"),
		QImage(":/pacman9.png"),
		QImage(":/pacman10.png"),
		QImage(":/pacman11.png"),
		QImage(":/pacman12.png")
	};

	{
		GHOST_RED = QImage(":/ghost.png");
		auto colors = GHOST_RED.colorTable();
		int color = 0;
		for (int i = 0; i < colors.size(); i++) {
			if (colors[i] == qRgb(255, 0, 0)) {
				color = i;
			}
		}

		GHOST_AMBUSH = GHOST_RED.copy();
		colors[color] = QColor("#ffb9ff").rgb();
		GHOST_AMBUSH.setColorTable(colors);

		GHOST_WHIMSICAL = GHOST_RED.copy();
		colors[color] = QColor("#00ffff").rgb();
		GHOST_WHIMSICAL.setColorTable(colors);

		GHOST_IGNORANT = GHOST_RED.copy();
		colors[color] = QColor("#ffb951").rgb();
		GHOST_IGNORANT.setColorTable(colors);
	}
	{
		GHOST_FRIGHTENED = QImage(":/frightened.png");
		auto colors = GHOST_RED.colorTable();
		int color = 0;
		for (int i = 0; i < colors.size(); i++) {
			if (colors[i] == qRgb(255, 0, 0)) {
				color = i;
			}
		}

		colors[color] = QColor("#2121ff").rgb();
		GHOST_FRIGHTENED.setColorTable(colors);

		GHOST_BLINK = GHOST_FRIGHTENED.copy();
		colors[color] = QColor("#e0ddff").rgb();
		GHOST_BLINK.setColorTable(colors);
	}
}