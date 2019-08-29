#include "People.h"
#include "Game.h"

People::People()
{
	mX = mY = 0;
	mSpeed = 1;
	mState = true;
}

People::People(int column, int row)
{
	mX = column;
	mY = row;
	mSpeed = 1;
	mState = true;
}

People::People(const People &p)
{
	copyPeople(p);
}

People & People::operator=(const People &p)
{
	if (this != &p) {
		copyPeople(p);
	}
	return *this;
}

People::~People()
{
}

void People::copyPeople(const People &p)
{
	mX = p.mX;
	mY = p.mY;
	mSpeed = p.mSpeed;
	mState = p.mState;
}

void People::selfDraw(ConsoleHandle * &handle)
{
	int curRow = mY - height + 1;
	int i = curRow, j;
	for (; curRow <= mY; ++curRow) {
		handle->gotoXY(mX, curRow);
		for (j = 0; j < width; ++j)
			cout << form[curRow - i][j];
	}
}

void People::selfDrawToBuffer(Buffer &b)
{
	int mYinBuffer = mY - Game::TOP + ADDITIONAL_SPACE;
	int mXinBuffer = mX - Game::LEFT;
	int curRow = mYinBuffer - height + 1;
	int i = curRow, j, k;

	for (; curRow <= mYinBuffer; ++curRow) {
		k = mXinBuffer;
		for (j = 0; j < width; ++j)
			b.draw(curRow, k++, form[curRow - i][j], default_ColorCode);
	}
}

void People::tell()
{
	PlaySound("sound/ouch.wav", NULL, SND_FILENAME);
}

void People::up()
{
	mY -= mSpeed;
}

void People::down()
{
	if (mY + mSpeed <= Game::BOTTOM)
		mY += mSpeed;
}

void People::left()
{
	if (mX - mSpeed >= Game::LEFT)
		mX -= mSpeed;
	else
		mX = Game::LEFT;
}

void People::right()
{
	if (mX + mSpeed <= Game::RIGHT - width + 1)
		mX += mSpeed;
	else
		mX = Game::RIGHT - width + 1;
}

void People::setPos(int x, int y)
{
	mX = x;
	mY = y;
}

bool People::isImpact(const Vehicle *obj, int num, bool soundOn)
{
	int x, y = obj[0].Y();
	if (y <= mY && y > mY - People::height) {
		for (int i = 0; i < num; ++i) {
			x = obj[i].X();
			if (mX >= x && mX < x + Vehicle::width) {
				mState = false;
				if (soundOn)
					tell();
				return true;
			}
		}
	}
	return false;
}

bool People::isImpact(const Animal *obj, int num, bool soundOn)
{
	int x, y = obj[0].Y();
	if (y <= mY + 1 && y > mY - People::height) {
		for (int i = 0; i < num; ++i) {
			x = obj[i].X();
			if (mX >= x && mX < x + Animal::width) {
				mState = false;
				if (soundOn)
					obj[i].tell();
				return true;
			}
		}
	}
	return false;
}

bool People::isFinish()
{
	return mY <= Game::TOP;
}

void People::dead()
{
	mState = false;
}

bool People::isDead()
{
	return mState == false;
}