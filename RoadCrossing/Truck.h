#pragma once
#include "Vehicle.h"
#include "Buffer.h"

class Truck: public SubVehicle<Truck>
{
public:
	static const char form[][4];
public:
	Truck();
	Truck(int, int);
	Truck(const Truck &);
	~Truck();

	Truck & operator=(const Truck &);
public:
	void selfDraw(ConsoleHandle * &) const;
	void selfDrawToBuffer(Buffer &, int) const;
};