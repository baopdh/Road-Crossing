#include "Animal.h"

Animal::Animal()
{
	mX = mY = 0;
}

Animal::Animal(int x, int y)
{
	mX = x;
	mY = y;
}

void Animal::copyAnimal(const Animal &a)
{
	mX = a.mX;
	mY = a.mY;
}

Animal::Animal(const Animal &a)
{
	copyAnimal(a);
}

Animal & Animal::operator=(const Animal &a)
{
	if (this != &a) {
		copyAnimal(a);
	}
	return *this;
}

Animal::~Animal()
{
}

void Animal::setPos(int x, int y)
{
	mX = x;
	mY = y;
}