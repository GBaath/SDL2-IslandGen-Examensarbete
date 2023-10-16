#pragma once
#ifndef tilemap_h
#define tilemap_h
#include "SDL.h"
#include "tile.h"
#include <map>




class Tilemap {
public:
	Tilemap();
	~Tilemap();
	void Init(SDL_Renderer* renderer);
	void RenderTiles(SDL_Renderer* renderer);
	void MakeIsland();
	void ClearIsland();
	void Clean();
	/*SDL_Rect GetTileTypeRect(Tile::TileType tileType) {
		return sourcemap[tileType];
	}*/
	void GetTileMapCordsOfTileType(int* x, int* y,Tile::TileType type) {
		switch (type)
		{
		case Tile::water_full:
			*x = 1;
			*y = 1;
			break;
		case Tile::forest_full:
			*x = 0;
			*y = 3;
			break;
		case Tile::toprightLand:
			*x = 2;
			*y = 0;
			break;
		case Tile::topLand:
			*x = 1;
			*y = 0;
			break;
		case Tile::topleftLand:
			*x = 0;
			*y = 0;
			break;
		case Tile::leftLand:
			*x = 0;
			*y = 1;
			break;
		case Tile::rightLand:
			*x = 2;
			*y = 1;
			break;
		case Tile::bottomleftLand:
			*x = 0;
			*y = 2;
			break;
		case Tile::bottomLand:
			*x = 1;
			*y = 2;
			break;
		case Tile::bottomrightLand:
			*x = 2;
			*y = 2;
			break;
		case Tile::topWater:
			*x = 3;
			*y = 1;
			break;
		case Tile::bottomWater:
			*x = 3;
			*y = 0;
			break;
		case Tile::rightWater:
			*x = 3;
			*y = 2;
			break;
		case Tile::leftWater:
			*x = 4;
			*y = 2;
			break;
		case Tile::pond:
			*x = 5;
			*y = 2;
			break;
		default:
			break;
		}

	}

	const static int WIDTH = 800;
	const static int HEIGHT = 640;
	const static int TILESIZE = 32;
	Tile* tilemap[WIDTH / TILESIZE][HEIGHT / TILESIZE];

private:
	int tileWidth, tileHeight = 0;
	int groundSize = 0;
	int maxGroundSize = 140;

	int holderx = 0;
	int holdery = 0;
	int* outx = &holderx;
	int* outy = &holdery;
	//used for coordinate management of source image
	const static int SOURCETILESX = 9,
		SOURCETILESY = 4;

	SDL_Surface* tilemapSurface = nullptr;
	SDL_Texture* tilemapTexture = nullptr;

	SDL_Rect tile[WIDTH / TILESIZE][HEIGHT / TILESIZE];

	SDL_Rect sourceTiles[SOURCETILESX][SOURCETILESY];
};
#endif "tilemap_h"