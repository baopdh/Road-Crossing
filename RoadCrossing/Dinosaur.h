#pragma once
#include "Animal.h"
#include "Buffer.h"
#include <iostream>
using namespace std;

class Dinosaur: public SubAnimal<Dinosaur>
{
public:
	static const char form[][3];
public:
	Dinosaur();
	Dinosaur(int, int);
	Dinosaur(const Dinosaur &);
	~Dinosaur();

	Dinosaur & operator=(const Dinosaur &);
public:
	void selfDraw(ConsoleHandle * &) const;
	void selfDrawToBuffer(Buffer &, int) const;
	void move();
	void tell() const;
};