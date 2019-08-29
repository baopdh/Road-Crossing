#pragma once
#include "Animal.h"
#include "Buffer.h"
#include <iostream>
using namespace std;

class Bird: public SubAnimal<Bird>
{
public:
	static const char form[][3];
public:
	Bird();
	Bird(int, int);
	Bird(const Bird &);
	~Bird();

	Bird & operator=(const Bird &);
public:
	void selfDraw(ConsoleHandle * &) const;
	void selfDrawToBuffer(Buffer &, int) const;
	void move();
	void tell() const;
};