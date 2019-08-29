#pragma once
#include "ConsoleHandle.h"
#pragma comment(lib, "winmm.lib")

class Animal
{
public:
	static const int width = 3;
	static const int height = 2;
private:
	void copyAnimal(const Animal &);
protected:
	int mX, mY;
public:
	Animal();
	Animal(int, int);
	Animal(const Animal &);
	virtual ~Animal();

	Animal & operator=(const Animal &);
public:
	virtual void selfDraw(ConsoleHandle * &) const = 0;
	virtual void move() = 0;
	virtual void tell() const = 0;
public:
	void setPos(int, int);
	int X() const { return mX; }
	int Y() const { return mY; }
};

template <class T>
class SubAnimal : public Animal
{
protected:
	static int mSpeed;
private:
	friend T;
	SubAnimal() { mSpeed = 1; }
	SubAnimal(int c, int r) : Animal(c, r) { mSpeed = 1; }
	SubAnimal(const SubAnimal<T> &a) : Animal(a) { mSpeed = a.mSpeed; }
	virtual ~SubAnimal() {}

	SubAnimal<T> & operator=(const SubAnimal<T> &a) {
		if (this != &a) {
			Animal::operator=(a);
			mSpeed = a.mSpeed;
		}
		return *this;
	}
public:
	void setSpeed(int s) {
		mSpeed = s;
	}

	void changeSpeed(int change) {
		mSpeed += change;
	}
};