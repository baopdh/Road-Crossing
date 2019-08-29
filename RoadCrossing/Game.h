#pragma once
#include "People.h"
#include "Bird.h"
#include "Dinosaur.h"
#include "Car.h"
#include "Truck.h"
#include <conio.h>
#include <thread>
#include <sstream>
#include <fstream>
using namespace std;
#define ADDITIONAL_SPACE 3
#define PROOF "ROADCROSSING-SAVE"

class Game
{
public:
	//Tọa độ khung game
	static const int MAX_LEVEL;
	static const int LEFT;
	static const int RIGHT;
	static const int TOP;
	static const int BOTTOM;
	//----------------------------
	static const char TITLE[][82];
	static const string MENU[];
	static const string INSTRUCTIONS[];
	static const string SETTINGS[];
private:
	People *mainChar;
	int numOfTrucks;
	Truck *m_trucks;
	int numOfCars;
	Car *m_cars;
	int numOfDinosaurs;
	Dinosaur *m_dinosaurs;
	int numOfBirds;
	Bird *m_birds;
	int m_isRunning;
	Buffer *playFrame;
	int m_currentLevel;

	void copyGame(const Game &);
	void clearGame();

	static void drawRectangle(ConsoleHandle *, int, int, int, int);//Vẽ hình chữ nhật bao quanh 2 đỉnh tham số (trái trên, phải dưới)
	static void removeRectangle(ConsoleHandle *, int, int, int, int);
	void soundSetting(ConsoleHandle *, bool &);
	int levelSetting(ConsoleHandle *, int);
	int crashPos(Animal *, int);
	int crashPos(Vehicle *, int);
	int crashPos();
public:
	Game();
	Game(int);
	Game(const Game &);
	~Game();

	Game & operator=(const Game &);
public:
	void drawPlayFrame(ConsoleHandle * &);
	void drawPlayFrameToBuffer();
	void drawBuffer(ConsoleHandle * &);
	void drawTitle(int, int, ConsoleHandle * &);
	void drawMenu(ConsoleHandle * &, const string [], const int, const int, int, int);
	void drawInstruction(ConsoleHandle * &, const string [], const int, const int);
	void drawLevel(ConsoleHandle * &);
	void drawGame(int, ConsoleHandle * &);

	int getLevel() const { return m_currentLevel; }
	People *getPeople() const { return mainChar; }
	Vehicle *getTrucks() const { return m_trucks; }
	Vehicle *getCars() const { return m_cars; }
	Animal *getDinosaurs() const { return m_dinosaurs; }
	Animal *getBirds() const { return m_birds; }
	int NumOfTrucks() const { return numOfTrucks; }
	int NumOfCars() const { return numOfCars; }
	int NumOfDinosaurs() const { return numOfDinosaurs; }
	int NumOfBirds() const { return numOfBirds; }
	bool isRunning() const { return m_isRunning; }

	void clearMenuArea(ConsoleHandle *);
	void resetGame(int);
	void exitGame(thread *);
	void startGame();
	string loadGameMenu(ConsoleHandle *);
	bool loadGame(const char *file);
	string saveGameMenu(ConsoleHandle *);
	bool saveGame(const char *file);
	void pauseGame();
	void resumeGame();
	void settings(ConsoleHandle *, bool &, int &);
	void explode(ConsoleHandle *, bool soundOn);
	static void progressBar(ConsoleHandle *, const int, const int, int);
	bool askLast(ConsoleHandle *);
	
	void updatePosPeople(char, thread &, bool);
	void updatePosVehicle();
	void updatePosAnimal();
};