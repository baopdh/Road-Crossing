#pragma once
#include "Vehicle.h"
#include "Buffer.h"

class Car: public SubVehicle<Car>
{
public:
	static const char form[][4];
	static const int width = 4;
	static const int height = 1;
public:
	Car();
	Car(int, int);
	Car(const Car &);
	~Car();

	Car & operator=(const Car &);
public:
	void selfDraw(ConsoleHandle * &) const;
	void selfDrawToBuffer(Buffer &, int) const;
};

