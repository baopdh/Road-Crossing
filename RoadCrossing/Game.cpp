#include "Game.h"

Game::Game()
{
	srand(time(NULL));
	resetGame(1);
}

Game::Game(int level)
{
	srand(time(NULL));
	resetGame(level);
}

template <class T>
void createObstacles(T * &m_objs, int num, int row, int col, int distance) {
	m_objs = new T[num];
	for (int i = 0; i < num; ++i, col += distance)
		m_objs[i].setPos(col, row);
}

void Game::resetGame(int level) {
	clearGame();
	m_currentLevel = level;
	m_isRunning = true;
	mainChar = new People((RIGHT + LEFT) / 2, BOTTOM);
	playFrame = new Buffer(Game::BOTTOM - Game::TOP + 1 + ADDITIONAL_SPACE, Game::RIGHT - Game::LEFT + 1);

	int interval = (level - 1) / 3 + 1;
	int numOfVehicles = 2 + interval;
	int carDistance = (Game::RIGHT - Game::LEFT - 1) / numOfVehicles;
	int laneDistance = (Game::BOTTOM - Game::TOP) / 6;
	int i;

	int row = Game::BOTTOM - 1 - laneDistance;
	numOfTrucks = numOfVehicles;
	createObstacles(m_trucks, numOfTrucks, row, Game::LEFT + 1, carDistance);

	row -= laneDistance;
	numOfCars = numOfVehicles;
	createObstacles(m_cars, numOfCars, row, (((Game::LEFT + 1) << 1) + carDistance) >> 1, carDistance);

	row -= laneDistance;
	numOfDinosaurs = numOfVehicles;
	createObstacles(m_dinosaurs, numOfDinosaurs, row, Game::LEFT + 1, carDistance);

	row -= laneDistance;
	numOfBirds = numOfVehicles;
	createObstacles(m_birds, numOfBirds, row, (((Game::LEFT + 1) << 1) + carDistance) >> 1, carDistance);

	interval = level % 3;
	if (!interval)
		interval = 3;

	if (interval != 3)
		mainChar->setSpeed(interval);
	else
		mainChar->setSpeed(2);

	m_trucks[0].setSpeed(interval);
	m_cars[0].setSpeed(-interval);

	m_dinosaurs[0].setSpeed(interval);
	m_birds[0].setSpeed(-interval);

	delete Car::lightState;
	Car::lightState = new TrafficLightStateOn<Car>();
	delete Truck::lightState;
	Truck::lightState = new TrafficLightStateOn<Truck>();
}

void Game::clearGame() {
	delete mainChar;
	delete playFrame;
	delete[]m_trucks;
	delete[]m_cars;
	delete[]m_dinosaurs;
	delete[]m_birds;
}

Game::~Game()
{
	clearGame();
}

Game::Game(const Game &g)
{
	copyGame(g);
}

void Game::copyGame(const Game &g)
{
	mainChar = new People(*g.mainChar);
	int i;
	
	numOfTrucks = g.numOfTrucks;
	m_trucks = new Truck[numOfTrucks];
	for (i = 0; i < numOfTrucks; ++i)
		m_trucks[i] = g.m_trucks[i];

	numOfCars = g.numOfCars;
	m_cars = new Car[numOfCars];
	for (i = 0; i < numOfCars; ++i)
		m_cars[i] = g.m_cars[i];

	numOfDinosaurs = g.numOfDinosaurs;
	m_dinosaurs = new Dinosaur[numOfDinosaurs];
	for (i = 0; i < numOfDinosaurs; ++i)
		m_dinosaurs[i] = g.m_dinosaurs[i];

	numOfBirds = g.numOfBirds;
	m_birds = new Bird[numOfBirds];
	for (i = 0; i < numOfBirds; ++i)
		m_birds[i] = g.m_birds[i];
}

Game & Game::operator=(const Game &g)
{
	if (this != &g) {
		clearGame();
		copyGame(g);
	}

	return *this;
}

void Game::drawPlayFrame(ConsoleHandle * &handle)
{
	int i, j, width = RIGHT - LEFT - 1;
	int row = TOP;
	handle->gotoXY(LEFT, row++);
	//Vẽ biên trên
	cout << char(218);
	for (i = 0; i < width; ++i)
		cout << char(196);
	cout << char(191);
	//Vẽ phần giữa
	int littleHeight = (BOTTOM - TOP) / 6 - 1;
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < littleHeight; ++j) {
			handle->gotoXY(LEFT, row);
			cout << char(179);
			handle->gotoXY(RIGHT, row++);
			cout << char(179);
		}
		handle->gotoXY(LEFT, row++);
		cout << char(195);
		for (j = 0; j < width; ++j)
			cout << char(196);
		cout << char(180);
	}
	//Vẽ phần dưới
	for (j = 0; j < littleHeight; ++j) {
		handle->gotoXY(LEFT, row);
		cout << char(179);
		handle->gotoXY(RIGHT, row++);
		cout << char(179);
	}
	handle->gotoXY(LEFT, row);
	cout << char(192);
	for (i = 0; i < width; ++i)
		cout << char(196);
	cout << char(217);
}

void Game::drawPlayFrameToBuffer()
{
	int i, j, width = playFrame->width() - 2;
	int row = ADDITIONAL_SPACE + 1;
	//Vẽ biên trên
	playFrame->draw(ADDITIONAL_SPACE, 0, 218, ColorCode_White);
	for (i = 1; i <= width; ++i)
		playFrame->draw(ADDITIONAL_SPACE, i, 196, ColorCode_White);
	playFrame->draw(ADDITIONAL_SPACE, i, 191, ColorCode_White);
	//Vẽ phần giữa
	int littleHeight = (BOTTOM - TOP) / 6 - 1;
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < littleHeight; ++j) {
			playFrame->draw(row, 0, 179, ColorCode_White);
			playFrame->draw(row++, width + 1, 179, ColorCode_White);
		}

		playFrame->draw(row, 0, 195, ColorCode_White);
		for (j = 1; j <= width; ++j)
			playFrame->draw(row, j, 196, ColorCode_White);
		playFrame->draw(row++, j, 180, ColorCode_White);
	}
	//Vẽ phần dưới
	for (j = 0; j < littleHeight; ++j) {
		playFrame->draw(row, 0, 179, ColorCode_White);
		playFrame->draw(row++, width + 1, 179, ColorCode_White);
	}
	playFrame->draw(row, 0, 192, ColorCode_White);
	for (i = 1; i <= width; ++i) {
		playFrame->draw(row, i, 196, ColorCode_White);
	}
	playFrame->draw(row, i, 217, ColorCode_White);
}

void Game::drawTitle(int height, int width, ConsoleHandle * &handle) {
	int row = (TOP - height) / 2;
	int col = (RIGHT + LEFT - width) / 2;
	int i, j;
	for (i = 0; i < height; ++i) {
		handle->gotoXY(col, row++);
		for (j = 0; j < width; ++j) {
			handle->textcolor(rand() % 8 + 8);
			cout << TITLE[i][j];
		}
	}
	handle->textcolor(default_ColorCode);
}

void Game::drawMenu(ConsoleHandle * &handle, const string a[], const int height = 3, const int width = 11, int selecting = 0, int color = ColorCode_Green) {
	int row = (TOP + BOTTOM - height) / 2 - 2;
	int col = (LEFT - width) / 2;
	handle->textcolor(ColorCode_White);
	for (int i = 0; i < 3; ++i) {
		handle->gotoXY(col, row++);
		if (i != selecting) {
			cout << a[i].c_str();
		}
		else {
			handle->textcolor(color);
			cout << a[i].c_str();
			handle->textcolor(ColorCode_White);
		}
	}
}

void Game::drawLevel(ConsoleHandle * &handle)
{
	int row = (TOP + BOTTOM - 3) / 2 + 4;
	int col = (LEFT - 7) / 2;
	handle->gotoXY(col, row);
	handle->textcolor(ColorCode_Yellow);
	cout << "LEVEL: " << m_currentLevel;
	handle->textcolor(default_ColorCode);
}

void Game::drawInstruction(ConsoleHandle * &handle, const string s[], const int height = 7, const int width = 31) {
	int row = (TOP + BOTTOM - height) / 2;
	int col = (RIGHT + handle->width() - width) / 2;

	handle->gotoXY(col, row++);
	handle->textcolor(ColorCode_White);
	cout << s[0].c_str();

	for (int i = 1; i < height; ++i) {
		handle->gotoXY(col, row++);
		handle->textcolor(i + 7);
		cout << s[i].c_str();
	}
	handle->textcolor(default_ColorCode);
}

void Game::drawBuffer(ConsoleHandle * &handle) {
	int row = TOP - ADDITIONAL_SPACE, i = 0, j;
	while (row <= BOTTOM) {
		handle->gotoXY(LEFT, row++);
		for (j = 0; j < playFrame->width(); ++j) {
			handle->textcolor(playFrame->getColor(i, j));
			cout << playFrame->get(i, j);
		}
		++i;
	}
}

void Game::drawGame(int MenuChoice, ConsoleHandle * &handle) {
	//draw to buffer
	playFrame->clear();
	
	drawPlayFrameToBuffer();
	int i;
	for (i = 0; i < numOfTrucks; ++i)
		m_trucks[i].selfDrawToBuffer(*playFrame, ColorCode_Grey);
	for (i = 0; i < numOfCars; ++i)
		m_cars[i].selfDrawToBuffer(*playFrame, ColorCode_Blue);
	for (i = 0; i < numOfDinosaurs; ++i)
		m_dinosaurs[i].selfDrawToBuffer(*playFrame, ColorCode_DarkYellow);
	for (i = 0; i < numOfBirds; ++i)
		m_birds[i].selfDrawToBuffer(*playFrame, ColorCode_Pink);
	mainChar->selfDrawToBuffer(*playFrame);
	Car::drawTrafficLightToBuffer(RIGHT - 1 - LEFT, (BOTTOM + TOP) / 2 - TOP + ADDITIONAL_SPACE, playFrame);
	Truck::drawTrafficLightToBuffer(LEFT + 1 - LEFT, BOTTOM - 2 * ((BOTTOM - TOP) / 6) - TOP + ADDITIONAL_SPACE, playFrame);

	drawBuffer(handle);
	//--------------

	drawMenu(handle, MENU, 3, 11, MenuChoice);
}

void Game::startGame() {
	m_isRunning = true;
}

void Game::clearMenuArea(ConsoleHandle *handle) {
	handle->gotoXY(0, TOP);
	int i, j;
	for (i = TOP; i < BOTTOM; ++i) {
		for (j = 0; j < LEFT; ++j)
			cout << " ";
		cout << "\n";
	}
}

void Game::drawRectangle(ConsoleHandle *handle, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
	--topLeftX;
	--topLeftY;
	++bottomRightX;
	++bottomRightY;
	
	int i;

	handle->gotoXY(topLeftX, topLeftY);
	cout << char(218);
	for (i = topLeftX + 1; i < bottomRightX; ++i)
		cout << char(196);
	cout << char(191);

	for (i = topLeftY + 1; i < bottomRightY; ++i) {
		handle->gotoXY(topLeftX, i);
		cout << char(179);
		handle->gotoXY(bottomRightX, i);
		cout << char(179);
	}

	handle->gotoXY(topLeftX, bottomRightY);
	cout << char(192);
	for (i = topLeftX + 1; i < bottomRightX; ++i)
		cout << char(196);
	cout << char(217);
}

void Game::removeRectangle(ConsoleHandle *handle, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
	--topLeftX;
	--topLeftY;
	++bottomRightX;
	++bottomRightY;

	int i;

	handle->gotoXY(topLeftX, topLeftY);
	cout << " ";
	for (i = topLeftX + 1; i < bottomRightX; ++i)
		cout << " ";
	cout << " ";

	for (i = topLeftY + 1; i < bottomRightY; ++i) {
		handle->gotoXY(topLeftX, i);
		cout << " ";
		handle->gotoXY(bottomRightX, i);
		cout << " ";
	}

	handle->gotoXY(topLeftX, bottomRightY);
	cout << " ";
	for (i = topLeftX + 1; i < bottomRightX; ++i)
		cout << " ";
	cout << " ";
}

struct Pair {
	int x, y;
};

string Game::loadGameMenu(ConsoleHandle *handle) {
	clearMenuArea(handle);

	handle->gotoXY(LEFT / 2 - 20, (BOTTOM + TOP) / 2 - 3);
	cout << "Please enter the link to the saved file!";

	drawRectangle(handle, 1, (BOTTOM + TOP) / 2 - 1, LEFT - 2, (BOTTOM + TOP) / 2 - 1);

	string link = handle->textEdit((BOTTOM + TOP) / 2 - 1, 1, LEFT - 2);
	
	return link;
}

bool Game::loadGame(const char *file) {
	/*ifstream f(file, ios::in);
	if (!f)
		return false;

	string token;
	getline(f, token);
	if (token != PROOF) {
		f.close();
		return false;
	}

	f >> m_currentLevel;
	resetGame(m_currentLevel);
	
	int x, y;
	f >> x >> y;
	mainChar->setPos(x, y);
	int i;
	for (i = 0; i < numOfTrucks; ++i) {
		f >> x >> y;
		m_trucks[i].setPos(x, y);
	}
	for (i = 0; i < numOfCars; ++i) {
		f >> x >> y;
		m_cars[i].setPos(x, y);
	}
	for (i = 0; i < numOfDinosaurs; ++i) {
		f >> x >> y;
		m_dinosaurs[i].setPos(x, y);
	}
	for (i = 0; i < numOfBirds; ++i) {
		f >> x >> y;
		m_birds[i].setPos(x, y);
	}

	f >> x >> y;
	if (m_trucks[0].getSpeed() != x)
		m_trucks[0].goNext(m_currentLevel);

	if (m_cars[0].getSpeed() != y)
		m_cars[0].goNext(m_currentLevel);

	f.close();
	return true;*/

	FILE *f = fopen(file, "rb");
	if (!f)
		return false;

	char token[50];
	fread(token, sizeof(char), strlen(PROOF), f);
	token[strlen(PROOF)] = '\0';
	if (strcmp(token, PROOF)) {
		fclose(f);
		return false;
	}
		
	fread(&m_currentLevel, sizeof(int), 1, f);
	resetGame(m_currentLevel);

	Pair p;
	fread(&p, sizeof(Pair), 1, f);
	mainChar->setPos(p.x, p.y);
	int i;
	for (i = 0; i < numOfTrucks; ++i) {
		fread(&p, sizeof(Pair), 1, f);
		m_trucks[i].setPos(p.x, p.y);
	}
	for (i = 0; i < numOfCars; ++i) {
		fread(&p, sizeof(Pair), 1, f);
		m_cars[i].setPos(p.x, p.y);
	}
	for (i = 0; i < numOfDinosaurs; ++i) {
		fread(&p, sizeof(Pair), 1, f);
		m_dinosaurs[i].setPos(p.x, p.y);
	}
	for (i = 0; i < numOfBirds; ++i) {
		fread(&p, sizeof(Pair), 1, f);
		m_birds[i].setPos(p.x, p.y);
	}

	fread(&p, sizeof(Pair), 1, f);
	if (m_trucks[0].getSpeed() != p.x)
		m_trucks[0].goNext(m_currentLevel);

	if (m_cars[0].getSpeed() != p.y)
		m_cars[0].goNext(m_currentLevel);

	fclose(f);
	return true;
}

string Game::saveGameMenu(ConsoleHandle *handle) {
	clearMenuArea(handle);

	handle->gotoXY(LEFT / 2 - 18, (BOTTOM + TOP) / 2 - 3);
	cout << "Please enter the link to save data!";

	drawRectangle(handle, 1, (BOTTOM + TOP) / 2 - 1, LEFT - 2, (BOTTOM + TOP) / 2 - 1);

	string link = handle->textEdit((BOTTOM + TOP) / 2 - 1, 1, LEFT - 2);

	return link;
}

bool Game::saveGame(const char *file) {
	/*ofstream f(file, ios::out);
	if (!f)
		return false;

	f << PROOF << endl;
	f << m_currentLevel << endl;
	f << mainChar->X() << " " << mainChar->Y() << endl;
	
	int i;
	for (i = 0; i < numOfTrucks; ++i)
		f << m_trucks[i].X() << " " << m_trucks[i].Y() << endl;
	
	for (i = 0; i < numOfCars; ++i)
		f << m_cars[i].X() << " " << m_cars[i].Y() << endl;
	
	for (i = 0; i < numOfDinosaurs; ++i)
		f << m_dinosaurs[i].X() << " " << m_dinosaurs[i].Y() << endl;
	
	for (i = 0; i < numOfBirds; ++i)
		f << m_birds[i].X() << " " << m_birds[i].Y() << endl;
	
	f << m_trucks[0].getSpeed() << " " << m_cars[0].getSpeed();

	f.close();
	return true;*/

	FILE *f = fopen(file, "wb");
	if (!f)
		return false;

	fwrite(PROOF, sizeof(char), strlen(PROOF), f);
	fwrite(&m_currentLevel, sizeof(int), 1, f);

	Pair p;
	p.x = mainChar->X();
	p.y = mainChar->Y();
	fwrite(&p, sizeof(Pair), 1, f);

	int i;

	for (i = 0; i < numOfTrucks; ++i) {
		p.x = m_trucks[i].X();
		p.y = m_trucks[i].Y();
		fwrite(&p, sizeof(Pair), 1, f);
	}

	for (i = 0; i < numOfCars; ++i) {
		p.x = m_cars[i].X();
		p.y = m_cars[i].Y();
		fwrite(&p, sizeof(Pair), 1, f);
	}

	for (i = 0; i < numOfDinosaurs; ++i) {
		p.x = m_dinosaurs[i].X();
		p.y = m_dinosaurs[i].Y();
		fwrite(&p, sizeof(Pair), 1, f);
	}

	for (i = 0; i < numOfBirds; ++i) {
		p.x = m_birds[i].X();
		p.y = m_birds[i].Y();
		fwrite(&p, sizeof(Pair), 1, f);
	}

	p.x = m_trucks[0].getSpeed();
	p.y = m_cars[0].getSpeed();
	fwrite(&p, sizeof(Pair), 1, f);

	fclose(f);
	return true;
}

void Game::soundSetting(ConsoleHandle *handle, bool &soundOn) {
	int row = (TOP + BOTTOM - 3) / 2 - 1;
	int col = (LEFT - 25) / 2 + 13;
	handle->textcolor(ColorCode_Cyan);
	drawRectangle(handle, col, row, col + 6, row);
	handle->textcolor(ColorCode_White);

	char c;
	do {
		handle->gotoXY(col, row);
		if (soundOn) {
			cout << char(174);
			handle->textcolor(ColorCode_Green);
			cout << " O N ";
			handle->textcolor(ColorCode_White);
			cout << char(175);
		}
		else {
			cout << char(174);
			handle->textcolor(ColorCode_Red);
			cout << " OFF ";
			handle->textcolor(ColorCode_White);
			cout << char(175);
		}

		c = _getch();
		if (c == -32) {
			c = _getch();
			if (c == key_RightArrow || c == key_LeftArrow)
				soundOn = !soundOn;
		}

	} while (c != key_Enter);
	
	clearMenuArea(handle);
}

int Game::levelSetting(ConsoleHandle *handle, int setLevel) {
	int row = (TOP + BOTTOM - 3) / 2 - 2;
	int col = (LEFT - 25) / 2 + 13;

	handle->gotoXY(col - 1, row + 2);
	handle->textcolor(ColorCode_DarkRed);
	cout << "Choose this level?";

	char c;
	int level = setLevel, colorYes = ColorCode_White, colorNo = ColorCode_White;
	bool choice = false;
	bool yes = true;
	do {
		if (!choice)
			handle->textcolor(ColorCode_Cyan);
		else
			handle->textcolor(ColorCode_White);
		drawRectangle(handle, col, row, col + 6, row);

		handle->gotoXY(col, row);
		handle->textcolor(ColorCode_Yellow);
		cout << char(174);
		handle->textcolor(ColorCode_Green);
		cout << "  " << level << "  ";
		handle->textcolor(ColorCode_Yellow);
		cout << char(175);

		handle->gotoXY(col + 10, row + 3);
		handle->textcolor(colorYes);
		cout << "Yes";
		handle->textcolor(ColorCode_White);
		cout << "/";
		handle->textcolor(colorNo);
		cout << "No";

		c = _getch();
		if (c == -32) {
			c = _getch();
			if (c == key_RightArrow || c == key_LeftArrow) {
				if (choice) {
					if (!yes) {
						colorYes = ColorCode_Green;
						colorNo = ColorCode_White;
						yes = true;
					}
					else {
						colorYes = ColorCode_White;
						colorNo = ColorCode_Red;
						yes = false;
					}
				}
				else {
					if (c == key_RightArrow)
						level = level % MAX_LEVEL + 1;
					else {
						--level;
						level = level == 0 ? MAX_LEVEL : level;
					}
				}
			}
			else if (c == key_UpArrow || c == key_DownArrow) {
				if (choice)
					choice = false;
				else {
					choice = true;
					if (yes) {
						colorYes = ColorCode_Green;
						colorNo = ColorCode_White;
					}
					else {
						colorYes = ColorCode_White;
						colorNo = ColorCode_Red;
					}
				}
			}
		}

	} while (c != key_Enter || !choice);

	clearMenuArea(handle);
	if (yes)
		return level;
	return -1;
}

void Game::settings(ConsoleHandle *handle, bool &soundOn, int &setLevel) {
	clearMenuArea(handle);

	int menuChoice = 0;
	int getKey;

	while (true) {
		drawMenu(handle, SETTINGS, 3, 25, menuChoice, ColorCode_Pink);
		getKey = toupper(_getch());
		if (getKey == key_UpArrow || getKey == key_DownArrow) {
			menuChoice += (getKey - 76) / 4 + 3;
			menuChoice %= 3;
		}
		else if (getKey == key_Enter) {
			if (!menuChoice)
				soundSetting(handle, soundOn);
			else
				if (menuChoice == 1) {
					int chosen = levelSetting(handle, setLevel);
					if (chosen != -1)
						setLevel = chosen;
				}
				else {
					break;
				}
		}
	}

	clearMenuArea(handle);
}

void Game::pauseGame() {
	m_isRunning = false;
}

void Game::resumeGame() {
	m_isRunning = true;
}

void moveSound()
{
	PlaySound("sound/cursor-move.wav", NULL, SND_FILENAME);
}

void Game::updatePosPeople(char key, thread &t, bool soundOn) {
	switch (key) {
	case 'W':
		if (soundOn) {
			t.detach();
			t = thread(moveSound);
		}
		mainChar->up();
		break;
	case 'A':
		if (soundOn) {
			t.detach();
			t = thread(moveSound);
		}
		mainChar->left();
		break;
	case 'S':
		if (soundOn) {
			t.detach();
			t = thread(moveSound);
		}
		mainChar->down();
		break;
	case 'D':
		if (soundOn) {
			t.detach();
			t = thread(moveSound);
		}
		mainChar->right();
	}
}

void Game::updatePosVehicle() {
	int i;
	for (i = 0; i < numOfTrucks; ++i)
		m_trucks[i].move();
	for (i = 0; i < numOfCars; ++i)
		m_cars[i].move();
}

void Game::updatePosAnimal() {
	int i;
	for (i = 0; i < numOfDinosaurs; ++i)
		m_dinosaurs[i].move();
	for (i = 0; i < numOfBirds; ++i)
		m_birds[i].move();
}

void Game::exitGame(thread *a) {
	m_isRunning = false;
	a->join();
}

int Game::crashPos(Animal *a, int num) {
	int i;
	int x = mainChar->X(), y = mainChar->Y();

	if (a[0].Y() <= y + 1 && a[0].Y() > y - People::height)
		for (i = 0; i < num; ++i) {
			if (x >= a[i].X() && x < a[i].X() + Animal::width) {
				if (y - 1 <= a[0].Y() && y - 1 > a[0].Y() - Animal::height)
					return (x * 1000 + y - 1);
				if (y <= a[0].Y() && y > a[0].Y() - Animal::height)
					return (x * 1000 + y);
				return (x * 1000 + y - 2);
			}
		}

	return -1;
}

int Game::crashPos(Vehicle *a, int num) {
	int i;
	int x = mainChar->X(), y = mainChar->Y();

	if (a[0].Y() <= y + 1 && a[0].Y() > y - People::height)
		for (i = 0; i < num; ++i) {
			if (x >= a[i].X() && x < a[i].X() + Vehicle::width) {
				if (y - 1 <= a[0].Y() && y - 1 > a[0].Y() - Vehicle::height)
					return (x * 1000 + y - 1);
				if (y <= a[0].Y() && y > a[0].Y() - Vehicle::height)
					return (x * 1000 + y);
				return (x * 1000 + y - 2);
			}
		}

	return -1;
}

int Game::crashPos() {
	int i;
	i = crashPos(m_trucks, numOfTrucks);
	if (i != -1)
		return i;
	i = crashPos(m_cars, numOfCars);
	if (i != -1)
		return i;
	i = crashPos(m_birds, numOfBirds);
	if (i != -1)
		return i;
	i = crashPos(m_dinosaurs, numOfDinosaurs);
	return i;
}

void draw(int x, int y, ConsoleHandle *handle, int restore[][3], int colors[][3]) {
	int j;
	for (int i = 0; i < 3; ++i) {
		handle->gotoXY(x - 1, y - 1 + i);
		for (j = 0; j < 3; ++j) {
			handle->textcolor(colors[i][j]);
			cout << char(restore[i][j]);
		}
	}
}

void copyScene(int copy[][3], int copyColors[][3], int paste[][3], int pasteColors[][3]) {
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			paste[i][j] = copy[i][j];
			pasteColors[i][j] = copyColors[i][j];
		}
}

void prepareScene1(int scene1[][3], int colors1[][3], int shape, int color) {
	scene1[0][1] = shape;
	colors1[0][1] = color;
	scene1[1][0] = shape;
	colors1[1][0] = color;
	scene1[1][2] = shape;
	colors1[1][2] = color;
	scene1[2][1] = shape;
	colors1[2][1] = color;
}

void prepareScene2(int scene2[][3], int colors2[][3], int shape, int color) {
	scene2[0][0] = shape;
	colors2[0][0] = color;
	scene2[0][2] = shape;
	colors2[0][2] = color;
	scene2[1][1] = shape;
	colors2[1][1] = color;
	scene2[2][0] = shape;
	colors2[2][0] = color;
	scene2[2][2] = shape;
	colors2[2][2] = color;
}

void makeSound() {
	PlaySound("sound/explode.wav", NULL, SND_FILENAME);
}

void Game::explode(ConsoleHandle *handle, bool soundOn) {
	int center = crashPos();
	if (center != -1) {
		int x = center / 1000;
		int y = center % 1000;
		int restore[3][3], colors[3][3], i;
		for (i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j) {
				restore[i][j] = playFrame->get(y - 1 + i - Game::TOP + ADDITIONAL_SPACE, x - 1 + j - Game::LEFT);
				colors[i][j] = playFrame->getColor(y - 1 + i - Game::TOP + ADDITIONAL_SPACE, x - 1 + j - Game::LEFT);
			}

		int scene1[3][3], colors1[3][3];
		copyScene(restore, colors, scene1, colors1);
		prepareScene1(scene1, colors1, 42, ColorCode_Yellow);

		int scene2[3][3], colors2[3][3];
		copyScene(restore, colors, scene2, colors2);
		prepareScene2(scene2, colors2, 42, ColorCode_Yellow);

		thread t;
		if (soundOn)
			t = thread(makeSound);
		for (i = 0; i < 2; ++i) {
			draw(x, y, handle, scene2, colors2);
			Sleep(400);
			draw(x, y, handle, scene1, colors1);
			Sleep(400);
		}
		if (t.joinable())
			t.join();
	}
}

void Game::progressBar(ConsoleHandle *handle, int xCenter, int yCenter, int time) {
	int half = 20, i;

	handle->gotoXY(xCenter - half, yCenter - 2);
	handle->textcolor(ColorCode_Green);
	cout << "Loading...";
	handle->gotoXY(xCenter + half, yCenter - 2);
	cout << "%";
	handle->textcolor(ColorCode_Pink);
	drawRectangle(handle, xCenter - half, yCenter, xCenter + half, yCenter);

	for (i = 0; i < (half * 2 + 1); ++i) {
		handle->gotoXY(xCenter - half + i, yCenter);
		handle->textcolor(ColorCode_Cyan);
		cout << char(219);

		handle->gotoXY(xCenter + half - 3, yCenter - 2);
		handle->textcolor(ColorCode_Green);
		printf("%3d", int((i + 1) * 1.0 / (half * 2 + 1) * 100));

		Sleep(time / (2 * half));
	}

	//Clear screen
	handle->gotoXY(xCenter - half, yCenter - 2);
	cout << "          ";
	removeRectangle(handle, xCenter - half, yCenter, xCenter + half, yCenter);
	handle->gotoXY(xCenter - half, yCenter);
	for (i = 0; i < (half * 2 + 1); ++i)
		cout << " ";
	handle->gotoXY(xCenter + half - 3, yCenter - 2);
	cout << "    ";
}

bool Game::askLast(ConsoleHandle *handle) {
	while (_kbhit())
		_getch();

	handle->gotoXY((LEFT + RIGHT) / 2 - 10, (TOP + BOTTOM) / 2 - 1);
	handle->textcolor(ColorCode_Cyan);
	cout << "Do you want to exit?";

	char c;
	int colorYes = ColorCode_Green, colorNo = ColorCode_White;
	bool yes = true;
	do {
		handle->gotoXY((LEFT + RIGHT) / 2 + 3, (TOP + BOTTOM) / 2 + 1);
		handle->textcolor(colorYes);
		cout << "Yes";
		handle->textcolor(ColorCode_White);
		cout << "/";
		handle->textcolor(colorNo);
		cout << "No";

		c = _getch();
		if (c == -32) {
			c = _getch();
			if (c == key_RightArrow || c == key_LeftArrow) {
					if (!yes) {
						colorYes = ColorCode_Green;
						colorNo = ColorCode_White;
						yes = true;
					}
					else {
						colorYes = ColorCode_White;
						colorNo = ColorCode_Red;
						yes = false;
					}
			}
		}

	} while (c != key_Enter);

	if (yes)
		return true;
	return false;
}