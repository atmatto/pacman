#include "ValueDisplay.h"

#include <QHBoxLayout>

ValueDisplay::ValueDisplay(QString label, int value, QWidget *parent) : QWidget(parent) {
	setStyleSheet("QLabel { font-size: 16pt; }");

	auto labelWidget = new QLabel(label + ": ");
    labelWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	labelWidget->setStyleSheet("font-weight: bold;");
    valueLabel = new QLabel();

    auto layout = new QHBoxLayout(this);
    layout->addWidget(labelWidget);
    layout->addWidget(valueLabel);

    update(value);
}

void ValueDisplay::update(int value) {
    valueLabel->setText(QString::number(value));
}