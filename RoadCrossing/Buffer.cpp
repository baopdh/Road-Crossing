#include "Buffer.h"

Buffer::Buffer()
{
	m_height = 0;
	m_width = 0;
}

Buffer::Buffer(int height, int width)
{
	m_height = height;
	m_width = width;
	curFrame = new char*[m_height];
	int i, j;
	for (i = 0; i < m_height; ++i) {
		curFrame[i] = new char[m_width];
		for (j = 0; j < m_width; ++j)
			curFrame[i][j] = ' ';
	}
	
	colors = new int*[m_height];
	for (i = 0; i < m_height; ++i) {
		colors[i] = new int[m_width];
		for (j = 0; j < m_width; ++j)
			colors[i][j] = default_ColorCode;
	}
}

Buffer::Buffer(int height, int width, char **frame)
{
	m_height = height;
	m_width = width;
	
	curFrame = new char*[m_height];
	int i, j;
	for (i = 0; i < m_height; ++i) {
		curFrame[i] = new char[m_width];
		for (j = 0; j < m_width; ++j)
			curFrame[i][j] = frame[i][j];
	}
	
	colors = new int*[m_height];
	for (i = 0; i < m_height; ++i) {
		colors[i] = new int[m_width];
		for (j = 0; j < m_width; ++j)
			colors[i][j] = default_ColorCode;
	}
}

Buffer::Buffer(const Buffer &b)
{
	copyBuffer(b);
}

Buffer::~Buffer()
{
	deleteBuffer();
}

void Buffer::copyBuffer(const Buffer &b)
{
	m_height = b.m_height;
	m_width = b.m_width;
	
	int i, j;

	curFrame = new char*[m_height];
	for (i = 0; i < m_height; ++i) {
		curFrame[i] = new char[m_width];
		for (j = 0; j < m_width; ++j)
			curFrame[i][j] = b.curFrame[i][j];
	}

	colors = new int*[m_height];
	for (i = 0; i < m_height; ++i) {
		colors[i] = new int[m_width];
		for (j = 0; j < m_width; ++j)
			colors[i][j] = b.colors[i][j];
	}
}

void Buffer::deleteBuffer()
{
	for (int i = 0; i < m_height; ++i)
		delete[]curFrame[i];
	delete[]curFrame;

	for (int i = 0; i < m_height; ++i)
		delete[]colors[i];
	delete[]colors;
}

Buffer & Buffer::operator=(const Buffer &b)
{
	if (this != &b) {
		deleteBuffer();
		copyBuffer(b);
	}

	return *this;
}

void Buffer::clear()
{
	int i, j;
	for (i = 0; i < m_height; ++i)
		for (j = 0; j < m_width; ++j)
			curFrame[i][j] = ' ';
}

void Buffer::draw(int i, int j, char c, int color)
{
	curFrame[i][j] = c;
	colors[i][j] = color;
}