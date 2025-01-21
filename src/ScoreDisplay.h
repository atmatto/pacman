#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class ScoreDisplay : public QWidget {
	Q_OBJECT
public:
	ScoreDisplay(const QString &label, QWidget *parent = nullptr) : QWidget(parent) {
		auto layout = new QHBoxLayout(this);
		auto labelWidget = new QLabel(label + ": ");
		labelWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		layout->addWidget(labelWidget);
		layout->addWidget(scoreLabel);

		updateLabel();
	}
private:
	void updateLabel() {
		scoreLabel->setText(QString::number(value));
	}

	int value = 0;
	QLabel *scoreLabel = new QLabel();
};

