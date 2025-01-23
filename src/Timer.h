#pragma once

#include <QTimer>
#include <functional>

// A wrapper for QTimer which can be paused and resumed.
class Timer : public QObject {
	Q_OBJECT
public:
	Timer(bool singleShot = false, QObject *parent = nullptr);

	template <typename T> static Timer *quick(QObject *context, double delay, T function) {
		auto *t = new Timer(true, context);
		connect(t, &Timer::timeout, context, [t, function]() {
			function();
			t->deleteLater();
		});
		t->start(delay);
		return t;
	}
public slots:
	// Hard actions:
	void start(double seconds);
	void stop();

	// Soft actions:
	void pause();
	void resume();

	double remaining();
signals:
	void timeout();
private:
	QTimer *timer = new QTimer(this);
	QTimer *resumeTimer = new QTimer(this);
	int remainingTime = 0; // ms
	bool canResume = false;
};