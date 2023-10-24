#pragma once
#ifndef tilemap_h
#define tilemap_h
#include "SDL.h"
#include "tile.h"
#include <map>
#include <vector>
#include "Calculator.h"
#include "decor.h"




class Tilemap {
public:
	Tilemap();
	~Tilemap();
	void Init(SDL_Renderer* renderer);
	void RenderTiles(SDL_Renderer* renderer);
	void MakeIsland();
	void SpawnForests(int startCount, int maxTileCount);
	void SpawnMountains(int count);
	void ClearIsland();
	void Clean();
	static void GetTileMapCordsOfTileType(int* x, int* y,Tile::TileType type) {
		switch (type)
		{
		case Tile::water_full:
			*x = 1;
			*y = 1;
			break;
		case Tile::land_full:
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
		case Tile::leftrightLand:
			*x = 1;
			*y = 3;
			break;
		case Tile::topbottomLand:
			*x = 2;
			*y = 3;
			break;
		case Tile::forest_pine:
			*x = 3;
			*y = 3;
			break;
		case Tile::forest_birch:
			*x = 4;
			*y = 3;
			break;
		case Tile::empty:
			break;
		default:
			break;
		}

	}
	static void GetDecorMapCordsOfTileType(int* x, int* y, Tile::DecorType type) {
		switch (type)
		{
		case Tile::empty:
			break;
		case Tile::pine:

			if (Calculator::GetRandomIndex(0, 1) > 0) {
				*x = 0;
				*y = 0;
			}
			else
			{
				*x = 0;
				*y = 0;
			}
			break;

		case Tile::birch:
			if (Calculator::GetRandomIndex(0, 1) > 0) {
				*x = 2;
				*y = 0;
			}
			else
			{
				*x = 2;
				*y = 0;
			}
			break;
		default:
			break;
		}
	}


	const static int WIDTH = 800;
	const static int HEIGHT = 640;
	const static int TILESIZE = 32;
	Tile* tilemap[WIDTH / TILESIZE][HEIGHT / TILESIZE];
	std::vector <Tile*> landTiles;

private:
	int tileWidth, tileHeight = 0;
	int groundSize = 0;
	int maxGroundSize = 140;

	//used for holding references to render tiles
	int holderx = 0;
	int holdery = 0;
	int* outx = &holderx;
	int* outy = &holdery;


	//used for coordinate management of source image
	const static int SOURCETILESX = 9,
		SOURCETILESY = 4;
	const static int DECORSOURCETILESX = 4, DECORSOURCETILESY = 4;
	const static int MOUNTAINSOURCETILES = 5;

	SDL_Surface* tilemapSurface = nullptr;
	SDL_Texture* tilemapTexture = nullptr;

	SDL_Surface* decormapSurface = nullptr;
	SDL_Texture* decormapTexture = nullptr;

	SDL_Surface* mountainSurface = nullptr;
	SDL_Texture* mountainTexture = nullptr;


	//the actual tilemap
	SDL_Rect tile[WIDTH / TILESIZE][HEIGHT / TILESIZE];
	std::vector <Moutain> mountains;

	//sourceimage
	SDL_Rect sourceTiles[SOURCETILESX][SOURCETILESY];
	SDL_Rect sourceTilesDecor[DECORSOURCETILESX][DECORSOURCETILESY];
	SDL_Rect sourceTilesMtn[5];
};
#endif "tilemap_h"