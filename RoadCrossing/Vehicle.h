#pragma once
#include "ConsoleHandle.h"
#include "Buffer.h"
#include <iostream>
using namespace std;

template <class T>
class TrafficLightState;

class Vehicle
{
public:
	static const int width = 4;
	static const int height = 1;
private:
	void copyVehicle(const Vehicle &);
protected:
	int mX, mY;
public:
	Vehicle();
	Vehicle(int, int);
	Vehicle(const Vehicle &);
	virtual ~Vehicle();

	Vehicle & operator=(const Vehicle &);
public:
	virtual void selfDraw(ConsoleHandle * &) const = 0;
	virtual void goNext(int) = 0;
	void setPos(int, int);
	int X() const { return mX; }
	int Y() const { return mY; }
};

template <class T>
class SubVehicle : public Vehicle
{
protected:
	static int mSpeed;
public:
	static TrafficLightState<T> *lightState;
private:
	friend T;
	SubVehicle() {
		mSpeed = 1;
	}
	SubVehicle(int c, int r) : Vehicle(c, r) {
		mSpeed = 1;
	}
	SubVehicle(const SubVehicle<T> &s) : Vehicle(s) {
		mSpeed = s.mSpeed;
	}
	virtual ~SubVehicle() {
	}

	SubVehicle<T> & operator=(const SubVehicle<T> &s) {
		if (this != &s) {
			Vehicle::operator=(s);
			mSpeed = s.mSpeed;
		}
		return *this;
	}
public:
	void setSpeed(int s) {
		mSpeed = s;
	}

	void move() {
		mX += mSpeed;
		if (mX >= Game::RIGHT)
			mX = Game::LEFT + 1 + mX - Game::RIGHT;
		if (mX <= Game::LEFT - width + 1)
			mX = Game::RIGHT - 1 - Game::LEFT + mX;
	}

	int getSpeed() {
		return mSpeed;
	}

	void changeSpeed(int change) {
		mSpeed += change;
	}

	static void setLightState(TrafficLightState<T> *state) {
		lightState = state;
	}

	static void drawTrafficLight(int x, int y, ConsoleHandle *handle) {
		lightState->selfDraw(x, y, handle);
	}

	static void drawTrafficLightToBuffer(int x, int y, Buffer *buffer) {
		lightState->selfDrawToBuffer(x, y, buffer);
	}

	void goNext(int level) {
		lightState->goNext(this, level);
	}
};

template <class T>
class TrafficLightState {
public:
	virtual void goNext(SubVehicle<T> *, int) = 0;
	virtual void selfDraw(int, int, ConsoleHandle *) = 0;
	virtual void selfDrawToBuffer(int, int, Buffer *) = 0;
};

template <class T>
class TrafficLightStateOn :public TrafficLightState<T> {
public:
	TrafficLightStateOn() {}
	virtual ~TrafficLightStateOn() {}
public:
	void goNext(SubVehicle<T> *v, int tmp) {
		SubVehicle<T>::setLightState(new TrafficLightStateOff<T>());
		v->setSpeed(0);
		delete this;
	}

	void selfDraw(int x, int y, ConsoleHandle *handle) {
		handle->gotoXY(x, y);
		handle->textcolor(ColorCode_Green);
		cout << char(220);

		handle->gotoXY(x, y + 1);
		handle->textcolor(ColorCode_Grey);
		cout << char(223);
	}

	void selfDrawToBuffer(int x, int y, Buffer *buffer) {
		buffer->draw(y, x, 220, ColorCode_Green);
		buffer->draw(y + 1, x, 223, ColorCode_Grey);
	}
};

template <class T>
class TrafficLightStateOff :public TrafficLightState<T> {
public:
	TrafficLightStateOff() {}
	virtual ~TrafficLightStateOff() {}
public:
	void goNext(SubVehicle<T> *v, int level) {
		SubVehicle<T>::setLightState(new TrafficLightStateOn<T>());

		int direction = 1;
		if (level < 0) {
			direction = -1;
			level = -level;
		}

		level %= 3;
		if (!level)
			level = 3;
		v->setSpeed(level * direction);

		delete this;
	}

	void selfDraw(int x, int y, ConsoleHandle *handle) {
		handle->gotoXY(x, y);
		handle->textcolor(ColorCode_Grey);
		cout << char(220);

		handle->gotoXY(x, y + 1);
		handle->textcolor(ColorCode_Red);
		cout << char(223);
	}

	void selfDrawToBuffer(int x, int y, Buffer *buffer) {
		buffer->draw(y, x, 220, ColorCode_Grey);
		buffer->draw(y + 1, x, 223, ColorCode_Red);
	}
};