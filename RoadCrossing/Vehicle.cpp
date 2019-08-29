#include "Vehicle.h"
#include "Game.h"

Vehicle::Vehicle()
{
	mX = mY = 0;
}

Vehicle::Vehicle(int column, int row)
{
	mX = column;
	mY = row;
}

Vehicle::Vehicle(const Vehicle &v)
{
	copyVehicle(v);
}

Vehicle & Vehicle::operator=(const Vehicle &v)
{
	if (this != &v) {
		copyVehicle(v);
	}
	return *this;
}

void Vehicle::copyVehicle(const Vehicle &v)
{
	mX = v.mX;
	mY = v.mY;
}

Vehicle::~Vehicle()
{
}

void Vehicle::setPos(int x, int y)
{
	mX = x;
	mY = y;
}