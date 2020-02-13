
// Tile.h
#pragma once
#include <string>

using namespace std;

class Tile
{
public:
	Tile();
	Tile(int);
	~Tile();

	void update();
	string out();

	int time;
	bool food;
};


