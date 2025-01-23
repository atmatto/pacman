#pragma once

#include <QImage>
#include <vector>

// Load the images from resources bundled with the executable.
void init_images();

extern QImage GHOST_RED;
extern QImage GHOST_AMBUSH;
extern QImage GHOST_WHIMSICAL;
extern QImage GHOST_IGNORANT;

extern QImage GHOST_FRIGHTENED;
extern QImage GHOST_BLINK;

extern std::vector<QImage> PACMAN;