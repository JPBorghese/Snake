// Tile.cpp

#include "Tile.h"
#include <string>

Tile::Tile() {
	time = 0;
	food = false;
}

Tile::Tile(int a = 0)
{
	time = a;
	food = false;
}

string Tile::out() {
	return "";
}

void Tile::update() {
	time--;
}

Tile::~Tile()
{
}