#include "ConsoleHandle.h"

ConsoleHandle::ConsoleHandle()
{
	int size = getSize();
	m_width = size / getSizeHelper;
	m_height = size % getSizeHelper;
}

ConsoleHandle::ConsoleHandle(int width, int height)
{
	resizeConsole(width, height);

	int size = getSize();
	m_width = size / getSizeHelper;
	m_height = size % getSizeHelper;;
}

ConsoleHandle::~ConsoleHandle()
{
}

void ConsoleHandle::resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void ConsoleHandle::textcolor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

void ConsoleHandle::fixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLongPtr(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLongPtr(consoleWindow, GWL_STYLE, style);
}

void ConsoleHandle::gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int ConsoleHandle::midVertical() const {
	return m_height / 2;
}

int ConsoleHandle::midHorizontal() const {
	return m_width / 2;
}

int ConsoleHandle::getSize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	return columns*getSizeHelper + rows;
}

string ConsoleHandle::getClipboardText() {
	string text;

	if (OpenClipboard(NULL))
	{
		HANDLE clip;
		clip = GetClipboardData(CF_TEXT);
		// lock and copy
		text = (LPSTR)GlobalLock(clip);
		// unlock 
		GlobalUnlock(clip);
		CloseClipboard();
	}

	return text;
}

void ConsoleHandle::clrscr()
{
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void ConsoleHandle::hideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void ConsoleHandle::showCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 10;
	info.bVisible = TRUE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void ConsoleHandle::printString(string s, int row, int begin, int end, size_t cursorPos, size_t cursorPosConsole) {
	hideCursor();
	--end;
	size_t len;
	
	gotoXY(begin, row);
	for (len = begin; len <= end; ++len)
		cout << " ";

	gotoXY(begin, row);
	len = s.length();
	if (len <= end - begin + 1)
		cout << s;
	else {
		for (size_t i = cursorPos - (cursorPosConsole - begin); i <= cursorPos - cursorPosConsole + end; ++i)
			cout << s[i];
	}
	showCursor();
}

void goLeft(size_t &cursor1, size_t &cursor2, int begin) {
	--cursor1;
	if (cursor1 < cursor2 - begin)
		--cursor2;
}

void goRight(size_t &cursor1, size_t &cursor2, int end, string s, int step) {
	cursor1 += step;
	cursor2 += step;
	if (cursor2 > end)
		cursor2 = end - (s.length() - cursor1);
}

string ConsoleHandle::textEdit(int row, int begin, int end) {
	showCursor();
	gotoXY(begin, row);
	string res;
	size_t cursorPos = 0, cursorPosConsole = begin;
	char c;
	do {
		c = _getch();
		if (c == Ctrl_V) {
			string paste = getClipboardText();
			res.insert(cursorPos, paste);
			goRight(cursorPos, cursorPosConsole, end, res, paste.length());
		}
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
			|| c == ':' || c == '/' || c == '.' || c == ' ') {
			res.insert(cursorPos, 1, c);
			goRight(cursorPos, cursorPosConsole, end, res, 1);
		}
		else if (c == key_Backspace) {
			if (cursorPos > 0) {
				res.erase(cursorPos - 1, 1);
				goLeft(cursorPos, cursorPosConsole, begin);
			}
		}
		else if (c == -32) {
			switch (_getch()) {
			case key_RightArrow:
				if (cursorPos < res.length()) {
					goRight(cursorPos, cursorPosConsole, end, res, 1);
				}
				break;
			case key_LeftArrow:
				if (cursorPos > 0) {
					goLeft(cursorPos, cursorPosConsole, begin);
				}
			}
		}

		printString(res, row, begin, end, cursorPos, cursorPosConsole);
		gotoXY(cursorPosConsole, row);
	} while (c != key_Enter);

	hideCursor();
	return res;
}