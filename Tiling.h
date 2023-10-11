#pragma once
#include "game.h"


class Tilemap {
public:
	Tilemap(int width, int height, int tileSize);
	~Tilemap();


private:
	Tile tiles[];
};
class Tile {

};