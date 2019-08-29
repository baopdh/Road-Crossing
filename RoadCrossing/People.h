#pragma once
#include "Vehicle.h"
#include "Animal.h"
#include "ConsoleHandle.h"
#include "Buffer.h"
#include <iostream>
using namespace std;

class People
{
public:
	static const char form[][1];
	static const int width = 1;
	static const int height = 3;
private:
	int mX, mY;
	int mSpeed;
	bool mState;

	void copyPeople(const People &);
public:
	People();
	People(int, int);
	~People();
	People(const People &);
	int X() { return mX; }
	int Y() { return mY; }
	void selfDraw(ConsoleHandle * &);
	void selfDrawToBuffer(Buffer &);
	void setSpeed(int speed) { mSpeed = speed; }
	void setPos(int, int);
	void tell();
	void up();
	void down();
	void left();
	void right();
	bool isImpact(const Vehicle *, int, bool);
	bool isImpact(const Animal *, int, bool);
	bool isFinish();
	bool isDead();
	void dead();

	People & operator=(const People &);
};

