// Snake.cpp 
//

#include "Tile.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <array>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <time.h>

using namespace std;

enum input { UP, DOWN, LEFT, RIGHT, NONE };

void printGrid();
void updateGrid();
void resetGrid();
void setInput();
void updatePos();
void clearConsole();
void placeFruit();
void shiftx(int x);
void shifty(int y);
void setConsoleColor(unsigned short);
void setScore(int s);
void ShowConsoleCursor(bool t);
double getSpeed();

static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

CONSOLE_SCREEN_BUFFER_INFO csbi;

const int snakeColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const int fruitColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
const int gridColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
const int white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const int gridx = 11;
const int gridy = 11;
const int offsetx = 2;
const int offsety = 2;
const int gridW = 6;
const double maxSpeed = .1;	// 10 updates a second

const COORD spot0 = { ((offsetx * 2) * gridW) + 3, offsety + 2 };

Tile grid[gridx][gridy];
input i = UP;
input prevInput = NONE;

int xpos, ypos, length;
int score;
bool endGame;

clock_t startTime;
clock_t endTime;

int main()
{
	ShowConsoleCursor(false);

	while (true) {

		endGame = false;

		printGrid();
		resetGrid();
		setInput();

		xpos = gridx / 2;
		ypos = gridy / 2;
		length = 3;
		score = 0;

		setConsoleColor(white);
		SetConsoleCursorPosition(hOut, { 53, 1 });
		cout << "Score : " << score;

		placeFruit();
		setInput();

		while (true) { // 0 to exit

			startTime = clock();

			updatePos();
			if (endGame) { break; }

			updateGrid();
			SetConsoleCursorPosition(hOut, { 0, 0 });

			endTime = clock();

			speed = 1 / (((double)((length - 3) / 2)) + 5);

			if (speed < maxSpeed) {
				speed = maxSpeed;
			}

			while((double)(endTime - startTime) / CLOCKS_PER_SEC < speed) {	// wait until the deltaTime > speed
				setInput();
				endTime = clock();
			}
		}

		setConsoleColor(white);
		SetConsoleCursorPosition(hOut, { 50, 28 });
		cout << "Try again? 1/0" << endl;

		SetConsoleCursorPosition(hOut, { 0, 30 });
		cout << "Made By : Jonathan Borghese";
		SetConsoleCursorPosition(hOut, { 0, 0 });

		char ch;
		do { ch = _getch(); } while (ch != '1' && ch != '0');

		if (ch == '1') {
			continue;
		}
		else {
			break; // end the program
		}
	}
	return 0;
}

void setScore(int x) {
	setConsoleColor(white);
	SetConsoleCursorPosition(hOut, { 61, 1 });
	cout << score;
}

void updatePos() {
	int oldx = xpos;
	int oldy = ypos;

	switch (i) {
	case UP:
		ypos--;
		break;

	case DOWN:
		ypos++;
		break;

	case LEFT:
		xpos--;
		break;

	case RIGHT:
		xpos++;
		break;
	}
	prevInput = i;

	// prevents array out of bounds
	if (ypos < 0 || ypos >= gridy) {
		ypos = oldy;
		xpos = oldx;
		endGame = true;
	}

	if (xpos < 0 || xpos >= gridx) {
		ypos = oldy;
		xpos = oldx;
		endGame = true;
	}

	// checks if next spot is food
	if (grid[xpos][ypos].food == true) {
		grid[xpos][ypos].food = false;
		placeFruit();
		length++;
		score++;
		setScore(score);
	}

	// checks if you hit yourself
	if (grid[xpos][ypos].time > 0) {
		endGame = true;
	}

	// set new grid spot value
	grid[xpos][ypos].time = length + 1;
}

void printGrid() {
	setConsoleColor(gridColor);
	clearConsole();
	shifty(offsety);
	shiftx(offsetx);

	for (int k = 0; k < gridx; k++) {
		cout << " -----";
	}
	cout << endl;

	for (int y = 0; y < gridy; y++) {
		shiftx(offsetx);
		for (int x = 0; x < gridx; x++) {
			cout << "|     ";
		}
		cout << "| " << endl;

		shiftx(offsetx);
		for (int k = 0; k < gridx; k++) {
			cout << " -----";
		}
		cout << endl;
	}
}

void setInput() {
	input dir = NONE;
	if ((GetAsyncKeyState(0x57) || GetAsyncKeyState(0x26)) && prevInput != DOWN) { // w (up)
		dir = UP;
	}
	if ((GetAsyncKeyState(0x53) || GetAsyncKeyState(0x28)) && prevInput != UP) { //s (down)
		dir = DOWN;
	}
	if ((GetAsyncKeyState(0x41) || GetAsyncKeyState(0x25)) && prevInput != RIGHT) { // a (left)
		dir = LEFT;
	}
	if ((GetAsyncKeyState(0x44) || GetAsyncKeyState(0x27)) && prevInput != LEFT) { // d (right)
		dir = RIGHT;
	}

	if (dir != NONE)
		i = dir;
}

void resetGrid() {
	for (int y = 0; y < gridy; y++) {
		for (int x = 0; x < gridx; x++) {
			grid[x][y] = Tile(0);
		}
	}
}

void clearConsole()
{
	COORD coord = { 0, 0 };
	DWORD count;

	if (GetConsoleScreenBufferInfo(hOut, &csbi))
	{
		FillConsoleOutputCharacter(hOut, (TCHAR)32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
		FillConsoleOutputAttribute(hOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

		SetConsoleCursorPosition(hOut, coord);
	}
	return;
}

void placeFruit() {

	bool isFood = false;
	short x, y;

	do {
		x = (rand() % gridx);
		y = (rand() % gridy);

		if (grid[x][y].food == false && grid[x][y].time < 1 && (x != xpos || y != ypos)) {
			grid[x][y].food = true;
			isFood = true;
		}

	} while (!isFood);

	SetConsoleCursorPosition(hOut, { spot0.X + (x * gridW), spot0.Y + (y * 2) });
	setConsoleColor(fruitColor);
	cout << "X";

}

void shiftx(int x) {
	while (x >= 0) {
		cout << '\t';
		x--;
	}
}

void shifty(int x) {
	while (x >= 0) {
		cout << endl;
		x--;
	}
}

void updateGrid() {
	setConsoleColor(snakeColor);

	for (short y = 0; y < gridy; y++) {
		for (short x = 0; x < gridx; x++) {
			int time = grid[x][y].time;
			if (time == length + 1) {
				SetConsoleCursorPosition(hOut, { spot0.X + (x * gridW), spot0.Y + (y * 2) });
				cout << 'O';
			}
			else if (time == 1) {
				SetConsoleCursorPosition(hOut, { spot0.X + (x * gridW), spot0.Y + (y * 2) });
				cout << " ";
			}
			grid[x][y].update();
		}
	}
}

void setConsoleColor(unsigned short colour)
{
	std::cout.flush();
	SetConsoleTextAttribute(hOut, colour);
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}