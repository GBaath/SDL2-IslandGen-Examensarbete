#pragma once
#ifndef tile_h
#define tile_h
class Tile {
public:
	enum TileType {grass,sand,water,forest};

	Tile();
	~Tile();
	
	void Init(int size,int xpos, int ypos);
	void SetTileType(TileType type) { tileType = type; }
	TileType GetTileType() { return tileType; }

	Tile* aboveTile = nullptr;
	Tile* belowTile = nullptr;
	Tile* rightTile = nullptr;
	Tile* leftTile = nullptr;

	SDL_Rect tileRect;
private:
	TileType tileType;
	int arrX, arrY;

};
#endif "tile.h"