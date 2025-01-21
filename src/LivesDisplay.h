#pragma once

#include <QWidget>

#include "ScoreDisplay.h"

// TODO: This is temporarily using ScoreDisplay
class LivesDisplay : public ScoreDisplay {
	Q_OBJECT
public:
	LivesDisplay(QWidget *parent = nullptr) : ScoreDisplay("Lives", parent) {}
};

