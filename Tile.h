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
	bool HasAsNeighbor(Tile* tile) {
		for (Tile* tile : neighborTiles) {
			if (tile == tile)
				return true;
		}
		return false;
	}
	TileType GetTileType() { return tileType; }

	Tile* neighborTiles[4]{nullptr,nullptr,nullptr,nullptr};


	SDL_Rect tileRect;
private:
	TileType tileType;
	int arrX, arrY;

};
#endif "tile.h"