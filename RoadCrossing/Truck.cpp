#include "Truck.h"
#include "Game.h"

Truck::Truck()
{
}

Truck::Truck(int column, int row): SubVehicle(column, row)
{
}

Truck::Truck(const Truck &t): SubVehicle(t)
{
}

Truck::~Truck()
{
}

Truck & Truck::operator=(const Truck &t)
{
	SubVehicle::operator=(t);
	return *this;
}

void Truck::selfDraw(ConsoleHandle * &handle) const
{
	int curRow = mY - height + 1;
	int i = curRow, j;
	for (; curRow <= mY; ++curRow) {
		for (j = 0; j < width; ++j) {
			if (mX + j < Game::LEFT) {
				handle->gotoXY(Game::RIGHT - Game::LEFT + mX + j + 1, curRow);
				cout << form[curRow - i][j];
			}
			else if (mX + j > Game::RIGHT) {
				handle->gotoXY(mX + j - Game::RIGHT + Game::LEFT - 1, curRow);
				cout << form[curRow - i][j];
			}
			else {
				handle->gotoXY(mX + j, curRow);
				cout << form[curRow - i][j];
			}
		}
	}
}

void Truck::selfDrawToBuffer(Buffer &b, int color) const {
	int mYinBuffer = mY - Game::TOP + ADDITIONAL_SPACE;
	int mXinBuffer = mX - Game::LEFT;
	int curRow = mYinBuffer - height + 1;

	int i = curRow, j;
	for (; curRow <= mYinBuffer; ++curRow) {
		for (j = 0; j < width; ++j) {
			if (mXinBuffer + j < 0)
				b.draw(curRow, b.width() + mXinBuffer + j, form[curRow - i][j], color);
			else if (mX + j > Game::RIGHT)
				b.draw(curRow, mXinBuffer + j - b.width(), form[curRow - i][j], color);
			else
				b.draw(curRow, mXinBuffer + j, form[curRow - i][j], color);
		}
	}
}