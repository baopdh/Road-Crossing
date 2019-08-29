#pragma once
#include <Windows.h>
#include <string>
#include <conio.h>
#include <iostream>
using namespace std;

#define ColorCode_Black			0
#define ColorCode_DarkBlue		1
#define ColorCode_DarkGreen		2
#define ColorCode_DarkCyan		3
#define ColorCode_DarkRed		4
#define ColorCode_DarkPink		5
#define ColorCode_DarkYellow	6
#define ColorCode_DarkWhite		7
#define ColorCode_Grey			8
#define ColorCode_Blue			9
#define ColorCode_Green			10
#define ColorCode_Cyan			11
#define ColorCode_Red			12
#define ColorCode_Pink			13
#define ColorCode_Yellow		14
#define ColorCode_White			15

#define default_ColorCode		7

#define key_Enter		13
#define Ctrl_V			22
#define key_Backspace	8
#define key_DownArrow	80
#define key_UpArrow		72
#define key_LeftArrow	75
#define key_RightArrow	77

class ConsoleHandle
{
public:
	static const int getSizeHelper = 1000;
	static const int rightShift = 20;
private:
	int m_width;
	int m_height;

	void resizeConsole(int, int);
	int getSize();
	string getClipboardText();
	void printString(string, int, int, int, size_t, size_t);
public:
	ConsoleHandle();
	ConsoleHandle(int, int);
	~ConsoleHandle();
public:
	void textcolor(int x);
	void fixConsoleWindow();
	void gotoXY(int x, int y);
	void clrscr();
	void hideCursor();
	void showCursor();
	int width() { return m_width; }
	int height() { return m_height; }
	int midVertical() const;
	int midHorizontal() const;
	string textEdit(int row, int begin, int end);
};