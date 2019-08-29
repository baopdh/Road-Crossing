#include "Game.h"
#include <Windows.h>
#include <conio.h>
#define HORIZONTAL_PLAY_FRAME_BROADENING 32
#define VERTICAL_PLAY_FRAME_BROADENING 12
#define SPACE_MAKING 5
using namespace std;

const char People::form[][1] = { {220},
								 {197},
								 {'^'} };

const char Car::form[][4] = { { 220,219,219,220 } };

const char Truck::form[][4] = { { 220,220,219,219 } };

const char Bird::form[][3] = { { 220,32,220 },
							  { 32,223,32 } };

const char Dinosaur::form[][3] = { { 32,32,220 },
								   { 220,219,223 } };

const char Game::TITLE[][82] = { { 32,95,95,95,95,95,32,32,     32,32,32,32,32,32,32,		 32,32,32,32,32,32,32,		  32,32,32,32,32,'_',32,	   32,32,32,32,32,  32,32,32,'_','_','_','_',32,	32,32,32,32,32,32,		 32,32,32,32,32,32,32,		 32,32,32,32,32,	   32,32,32,32,32,		 32,32,32,	  32,32,32,32,32,32,32,		   },
							    { 178,32,32,'_','_',32,92,32,	 32,32,32,32,32,32,32,		 32,32,32,32,32,32,32,		  32,32,32,32,178,32,178,	   32,32,32,32,32,  32,32,'/',32,'_','_','_',178,	32,32,32,32,32,32,		 32,32,32,32,32,32,32,		 32,32,32,32,32,	   32,32,32,32,32,		 32,'_',32,	  32,32,32,32,32,32,32,		   },
							    { 178,32,178,'_','_',')',32,')', 32,32,'_','_','_',32,32,	 32,32,'_','_',32,'_',32,	  32,32,'_','_',178,32,178,	   32,32,32,32,32,  32,'/',32,32,178,32,32,32,		32,'_',32,'_','_',32,	 32,32,'_','_','_',32,32,	 32,'_','_','_',32,    32,'_','_','_',32,	 '(','_',')', 32,'_',32,'_','_',32,32,	  32,32,'_','_',32,'_',32},
								{ 178,32,32,'_',32,32,'/',32,	 32,'/',32,'_',32,'\\',32,	 32,'/',32,'_','\'',32,178,	  32,'/',32,'_',32,32,178,	   32,32,32,32,32,  168,'?',32,32,178,32,32,32,		178,32,'\'','_','_',178, 32,'/',32,'_',32,'\\',32,	 '/',32,'_','_',178,   '/',32,'_','_',178,   178,32,178,  '|',32,'\'','_',32,'\\',32, 32,'/',32,'_','\'',32,178},
								{ 178,32,178,32,92,32,92,32,	 '(',32,'(','_',')',32,')',	 '(',32,'(','_',')',32,178,	  '(',32,'(','_',')',32,178,   32,32,32,32,32,  32,'\\',32,32,178,'_','_','_',  178,32,178,32,32,32,	 '(',32,'(','_',')',32,')',  '\\','_','_',32,'\\', '\\','_','_',32,'\\', 178,32,178,  '|',32,178,32,'|',32,'|',	  '(',32,'(','_',')',32,178},
							    { 178,'_',178,32,32,92,'_',92,	 32,'\\','_','_','_','/',32, 32,'\\','_','_',',','_',178, 32,'\\','_','_',',','_',178, 32,32,32,32,32,  32,32,'\\','_','_','_','_',178, 178,'_',178,32,32,32,	 32,'\\','_','_','_','/',32, 178,'_','_','_','/',  178,'_','_','_','/',  178,'_',178, '|','_',178,32,'|','_','|', 32,'\\','_','_',',',32,178},
								{ 32,32,32,32,32,32,32,32,		 32,32,32,32,32,32,32,		 32,32,32,32,32,32,32,		  32,32,32,32,32,32,32,		   32,32,32,32,32,  32,32,32,32,32,32,32,32,		32,32,32,32,32,32,		 32,32,32,32,32,32,32,		 32,32,32,32,32,	   32,32,32,32,32,		 32,32,32,	  32,32,32,32,32,32,32,		  32,178,'_','_','_','/'} };

const string Game::MENU[] = { "1. New Game","2. Load Game","3. Settings" };

const string Game::SETTINGS[] = { "1. Sound","2. Level","3. Back" };

const string Game::INSTRUCTIONS[] = { "\tControls",
	"Directional Buttons:",
	"   W A S D (up left down right)",
	"Pause Game:  P",
	"Resume Game: R",
	"Save Game:   L",
	"Load Game:   T",
	"Exit Game:   Esc"};

ConsoleHandle *handle = new ConsoleHandle(1300, 800);//Thay đổi kích thước màn hình

const int midHorizontal = handle->midHorizontal();
const int midVertical = handle->midVertical();

const int Game::LEFT = midHorizontal - HORIZONTAL_PLAY_FRAME_BROADENING;
const int Game::RIGHT = midHorizontal + HORIZONTAL_PLAY_FRAME_BROADENING;
const int Game::TOP = midVertical - VERTICAL_PLAY_FRAME_BROADENING + SPACE_MAKING;
const int Game::BOTTOM = midVertical + VERTICAL_PLAY_FRAME_BROADENING + SPACE_MAKING;

const int Game::MAX_LEVEL = ((RIGHT - LEFT - 1) / 11 - 2) * 3;//(Số lượng xe max - Số lượng xe bắt đầu + 1) * 3

int SubAnimal<Dinosaur>::mSpeed = 1;
int SubAnimal<Bird>::mSpeed = -1;
int SubVehicle<Truck>::mSpeed = 1;
int SubVehicle<Car>::mSpeed = -1;
TrafficLightState<Car> *Car::lightState = new TrafficLightStateOn<Car>();
TrafficLightState<Truck> *Truck::lightState = new TrafficLightStateOn<Truck>();

Game g(1);
char MOVING;
bool justWinOrDead = false, soundOn = true;
int menuChoice = 0, setLevel = 1;

void drawStartScreen() {
	g.drawTitle(7, 82, handle);
	g.drawInstruction(handle, Game::INSTRUCTIONS, 8, 31);
	g.drawMenu(handle, Game::MENU, 3, 11, menuChoice, ColorCode_Green);
	g.drawPlayFrame(handle);
}

void tmp() {
}

bool waitForDyingEffect = true;

void runGame() {
	thread sound(tmp);
	g.drawLevel(handle);
	while (g.isRunning()) {
		if (justWinOrDead) {
			MOVING = ' ';
			justWinOrDead = false;
		}
		if (!g.getPeople()->isDead()) {
			g.updatePosPeople(MOVING, sound, soundOn);
		}
		MOVING = ' ';
		g.updatePosVehicle();
		g.updatePosAnimal();
		g.drawGame(menuChoice, handle);

		if (g.getPeople()->isImpact(g.getTrucks(), g.NumOfTrucks(), soundOn)
			|| g.getPeople()->isImpact(g.getCars(), g.NumOfCars(), soundOn)
			|| g.getPeople()->isImpact(g.getDinosaurs(), g.NumOfDinosaurs(), soundOn)
			|| g.getPeople()->isImpact(g.getBirds(), g.NumOfBirds(), soundOn)) {
			//g.getPeople()->dead();
			g.pauseGame();
		}

		if (g.getPeople()->isDead()) {
			justWinOrDead = true;
			
			g.explode(handle, soundOn);
			handle->textcolor(ColorCode_Red);
			handle->gotoXY((Game::LEFT + Game::RIGHT) / 2 - 25, (Game::TOP + Game::BOTTOM) / 2 - 1);
			cout << "You died. Press Y to replay, or another key to exit.";
			waitForDyingEffect = false;
			break;
		}

		if (g.getPeople()->isFinish()) {
			justWinOrDead = true;
			handle->gotoXY((Game::LEFT + Game::RIGHT) / 2 - 20, (Game::TOP + Game::BOTTOM) / 2 - 1);
			handle->textcolor(ColorCode_Green);
			cout << "You crossed the road. What a success =))";
			Sleep(2000);
			g.resetGame(g.getLevel() % Game::MAX_LEVEL + 1);
			g.drawLevel(handle);
		}
		
		Sleep(90);
	}
	sound.join();
}

//run Menu while pausing game
void runMenu(thread &t1) {
	int getKey;
	string link;
	while (true) {
		g.drawMenu(handle, Game::MENU, 3, 11, menuChoice, ColorCode_Green);
		getKey = toupper(_getch());
		if (getKey == 'R') {
			g.resumeGame();
			t1.detach();
			t1 = thread(runGame);
			break;
		}
		if (getKey == key_UpArrow || getKey == key_DownArrow) {
			menuChoice += (getKey - 76) / 4 + 3;
			menuChoice %= 3;
		}
		else if (getKey == key_Enter) {
			if (!menuChoice) {
				g.resetGame(setLevel);
				t1.detach();
				t1 = thread(runGame);
				break;
			}
			else if (menuChoice == 1) {
				link = g.loadGameMenu(handle);
				bool loaded = g.loadGame(link.c_str());
				if (loaded) {
					g.clearMenuArea(handle);
					g.resumeGame();
					t1.detach();
					t1 = thread(runGame);
					break;
				}
				else {
					handle->gotoXY(Game::LEFT / 2 - 22, (Game::BOTTOM + Game::TOP) / 2 + 1);
					handle->textcolor(ColorCode_Red);
					cout << "Cannot open file or incorrect file structure.";
					Sleep(2000);
					g.clearMenuArea(handle);
				}
			}
			else {
				g.settings(handle, soundOn, setLevel);
			}
		}
	}
}

bool changingLight = true;

void changeLight() {
	while (changingLight) {
		Sleep(500);
		
		if (g.isRunning()) {
			if (rand() % 7 == 0)
				g.getTrucks()[0].goNext(g.getLevel());

			if (rand() % 7 == 0)
				g.getCars()[0].goNext(-g.getLevel());
		}
		
	}
}

int main() {
	srand(time(NULL) + rand());
	handle->fixConsoleWindow();
	handle->hideCursor();
	Game::progressBar(handle, midHorizontal, midVertical, 1000);

	int getKey;
	string link;

	//Lựa chọn lúc bắt đầu
	drawStartScreen();
	while (true) {
		g.drawMenu(handle, Game::MENU, 3, 11, menuChoice, ColorCode_Green);
		getKey = toupper(_getch());
		if (getKey == key_UpArrow || getKey == key_DownArrow) {
			menuChoice += (getKey - 76) / 4 + 3;
			menuChoice %= 3;
		}
		else if (getKey == key_Enter) {
			if (!menuChoice) {
				g.resetGame(setLevel);
				break;
			}
			else if (menuChoice == 1) {
					link = g.loadGameMenu(handle);
					bool loaded = g.loadGame(link.c_str());
					if (loaded) {
						g.clearMenuArea(handle);
						break;
					}
					else {
						handle->gotoXY(Game::LEFT / 2 - 22, (Game::BOTTOM + Game::TOP) / 2 + 1);
						handle->textcolor(ColorCode_Red);
						cout << "Cannot open file or incorrect file structure.";
						Sleep(2000);
						g.clearMenuArea(handle);
					}
				}
				else {
					g.settings(handle, soundOn, setLevel);
				}
		}
		else if (getKey == 27) {
			delete handle;
			return 0;
		}
	}

	thread light(changeLight);

	//Bắt đầu game
	thread t1(runGame);
	while (true) {
		getKey = toupper(_getch());
		if (!g.getPeople()->isDead()) {
			//getKey = toupper(_getch());
			if (getKey == 27) {
				changingLight = false;
				g.exitGame(&t1);
				break;
			}
			else if (getKey == 224) {
				getKey = _getch();
				if (getKey == key_UpArrow || getKey == key_DownArrow) {
					menuChoice += (getKey - 76) / 4 + 3;
					menuChoice %= 3;
				}
			}
			else if (getKey == 'P') {
				g.pauseGame();
			}
			else if (getKey == 'R') {
				if (!g.isRunning()) {
					g.resumeGame();
					t1.detach();
					t1 = thread(runGame);
				}
			}
			else if (getKey == 'L') {
				g.pauseGame();
				Sleep(500);
				link = g.saveGameMenu(handle);
				bool saved = g.saveGame(link.c_str());
				if (saved) {
					handle->gotoXY(Game::LEFT / 2 - 8, (Game::BOTTOM + Game::TOP) / 2 + 1);
					handle->textcolor(ColorCode_Green);
					cout << "Game data saved!";
					Sleep(2000);
					g.clearMenuArea(handle);
					runMenu(t1);
				}
				else {
					handle->gotoXY(Game::LEFT / 2 - 8, (Game::BOTTOM + Game::TOP) / 2 + 1);
					handle->textcolor(ColorCode_Red);
					cout << "Cannot open file.";
					Sleep(2000);
					g.clearMenuArea(handle);
					runMenu(t1);
				}
			}
			else if (getKey == 'T') {
				g.pauseGame();
				Sleep(500);
				link = g.loadGameMenu(handle);
				bool loaded = g.loadGame(link.c_str());
				if (loaded) {
					g.clearMenuArea(handle);
					g.resumeGame();
					t1.detach();
					t1 = thread(runGame);
				}
				else {
					handle->gotoXY(Game::LEFT / 2 - 22, (Game::BOTTOM + Game::TOP) / 2 + 1);
					handle->textcolor(ColorCode_Red);
					cout << "Cannot open file or incorrect file structure.";
					Sleep(2000);
					g.clearMenuArea(handle);
					runMenu(t1);
				}
			}
			else if (getKey == key_Enter && !g.getPeople()->isDead()) {
				g.pauseGame();
				Sleep(500);
				if (!menuChoice) {
					g.resetGame(setLevel);
					g.resumeGame();
					t1.detach();
					t1 = thread(runGame);
					continue;
				}
				else if (menuChoice == 1) {
					link = g.loadGameMenu(handle);
					bool loaded = g.loadGame(link.c_str());
					if (loaded) {
						g.clearMenuArea(handle);
						g.resumeGame();
						t1.detach();
						t1 = thread(runGame);
					}
					else {
						handle->gotoXY(Game::LEFT / 2 - 22, (Game::BOTTOM + Game::TOP) / 2 + 1);
						handle->textcolor(ColorCode_Red);
						cout << "Cannot open file or incorrect file structure.";
						Sleep(2000);
						g.clearMenuArea(handle);
						runMenu(t1);
					}
				}
				else {
					g.settings(handle, soundOn, setLevel);
					runMenu(t1);
				}
			}
			else if (g.isRunning()) {
				MOVING = getKey;
			}
		}
		else {
			while (waitForDyingEffect) {}
			if (getKey == 'Y') {
				g.resetGame(g.getLevel());
				t1.detach();
				t1 = thread(runGame);
			}
			else {
				g.drawGame(menuChoice, handle);
				if (g.askLast(handle)) {
					g.exitGame(&t1);
					break;
				}
				else {
					g.resetGame(g.getLevel());
					t1.detach();
					t1 = thread(runGame);
				}
			}
			waitForDyingEffect = true;
		}
	}

	delete handle;
	delete Car::lightState;
	delete Truck::lightState;

	changingLight = false;
	light.join();

	return 0;
}