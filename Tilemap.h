#pragma once
#ifndef tilemap_h
#define tilemap_h
#include "SDL.h"
#include "tile.h"




class Tilemap {
public:
	Tilemap();
	~Tilemap();
	void Init(SDL_Renderer* renderer);
	void RenderTiles(SDL_Renderer* renderer);
	void MakeIsland();
	void ClearIsland();
	void Clean();

	const static int WIDTH = 800;
	const static int HEIGHT = 640;
	const static int TILESIZE = 32;
	Tile* tilemap[WIDTH / TILESIZE][HEIGHT / TILESIZE];

private:
	int tileWidth, tileHeight = 0;
	int groundSize = 0;
	int maxGroundSize = 140;

	SDL_Surface* tilemapSurface = nullptr;
	SDL_Texture* tilemapTexture = nullptr;

	SDL_Rect tile[WIDTH / TILESIZE][HEIGHT / TILESIZE];

	SDL_Rect tile1;
	SDL_Rect tile2;
	SDL_Rect tile3;
	SDL_Rect tile4;
};
#endif "tilemap_h"