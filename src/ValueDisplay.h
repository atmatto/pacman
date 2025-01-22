#pragma once

#include <QWidget>
#include <QLabel>

class ValueDisplay : public QWidget {
	Q_OBJECT
public:
	ValueDisplay(QString label, int value = 0, QWidget *parent = nullptr);
public slots:
	void update(int value);
private:
	QLabel *valueLabel;
};
