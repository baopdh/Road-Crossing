#pragma once
#include "ConsoleHandle.h"

class Buffer
{
private:
	int m_height, m_width;
	char **curFrame;
	int **colors;
public:
	Buffer();
	Buffer(int, int);
	Buffer(int, int, char **);
	Buffer(const Buffer &);
	~Buffer();

	void copyBuffer(const Buffer &);
	void deleteBuffer();
public:
	int height() { return m_height; }
	int width() { return m_width; }
	char get(int i, int j) { return curFrame[i][j]; }
	int getColor(int i, int j) { return colors[i][j]; }

	void clear();
	void draw(int, int, char, int);

	Buffer & operator=(const Buffer &);
};