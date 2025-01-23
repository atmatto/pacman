#include "Timer.h"

Timer::Timer(bool singleShot, QObject *parent) : QObject(parent) {
	timer->setSingleShot(singleShot);
	connect(timer, &QTimer::timeout, this, &Timer::timeout);

	resumeTimer->setSingleShot(true);
	connect(resumeTimer, &QTimer::timeout, this, [this](){
		emit timeout();
		if (!timer->isSingleShot()) {
			timer->start();
		}
		canResume = false;
	});
}

void Timer::start(double seconds) {
	timer->start(seconds * 1000);
}

void Timer::stop() {
	resumeTimer->stop();
	timer->stop();
	remainingTime = 0;
	canResume = false;
}

void Timer::pause() {
	if (resumeTimer->isActive()) {
		remainingTime = resumeTimer->remainingTime();
		resumeTimer->stop();
		canResume = true;
	} else if (timer->isActive()) {
		remainingTime = timer->remainingTime();
		timer->stop();
		canResume = true;
	}
}

void Timer::resume() {
	if (canResume) {
		resumeTimer->start(remainingTime);
	}
}

double Timer::remaining() {
	return (resumeTimer->remainingTime() + timer->remainingTime()) / 1000.0;
}