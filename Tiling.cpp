#include "Tilemap.h"
#include <iostream>

Tilemap::Tilemap() {
	tileWidth = WIDTH / TILESIZE;
	tileHeight = HEIGHT / TILESIZE;
}
Tilemap::~Tilemap() {

}
void Tilemap::Init(SDL_Renderer* renderer) {
	tilemapSurface = SDL_LoadBMP("assets/Texture/Tiles.bmp");
	if (!tilemapSurface) {
		std::cerr << "Error loading BMP: " << SDL_GetError() << std::endl;
	}

	tilemapTexture = SDL_CreateTextureFromSurface(renderer, tilemapSurface);
	SDL_FreeSurface(tilemapSurface);
	if (!tilemapTexture) {
		std::cout << "notex" << std::endl;;
	}

	//set coords and sizes
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			tilemap[x][y] = new Tile();
			tilemap[x][y]->Init(TILESIZE,x*TILESIZE,y*TILESIZE);
		}
	}

	//todo turn into array make better storage
	tile1.x = 0;
	tile1.y = 0;
	tile1.w = TILESIZE;
	tile1.h = TILESIZE;
	tile2.x = TILESIZE;
	tile2.y = 0;
	tile2.w = TILESIZE;
	tile2.h = TILESIZE;
	tile3.x = 0;
	tile3.y = TILESIZE;
	tile3.w = TILESIZE;
	tile3.h = TILESIZE;
	tile4.x = TILESIZE;
	tile4.y = TILESIZE;
	tile4.w = TILESIZE;
	tile4.h = TILESIZE;

}
void Tilemap::RenderTiles(SDL_Renderer* renderer) {

	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			//todo make not hardcoded
			switch (tilemap[x][y]->GetTileType())
			{
			case Tile::TileType::forest:
				SDL_RenderCopy(renderer, tilemapTexture, &tile1, &tilemap[x][y]->tileRect);
				break;
			case Tile::TileType::sand:
				SDL_RenderCopy(renderer, tilemapTexture, &tile2, &tilemap[x][y]->tileRect);
				break;
			case Tile::TileType::grass:
				SDL_RenderCopy(renderer, tilemapTexture, &tile3, &tilemap[x][y]->tileRect);
				break;
			case Tile::TileType::water:
				SDL_RenderCopy(renderer, tilemapTexture, &tile4, &tilemap[x][y]->tileRect);
				break;
			default:
				break;
			}
		}
	}
}
void Tilemap::MakeIsland() {

	tilemap[tileWidth / 2][tileHeight / 2]->SetTileType(Tile::TileType::forest);

	//dont generate on edges
	for (int x = 1; x < (tileWidth-1); x++) {
		for (int y = 1; y < (tileHeight-1); y++) {
			
			Tile* tile = tilemap[x][y];


			tile->aboveTile = tilemap[x][y+1];
			tile->belowTile = tilemap[x][y-1];
			tile->rightTile = tilemap[x+1][y];
			tile->leftTile = tilemap[x-1][y];

			if ((tile->GetTileType()==Tile::TileType::forest) && groundSize<maxGroundSize) {
				if (tile->aboveTile->GetTileType() == Tile::TileType::water) {
					tile->aboveTile->SetTileType(Tile::TileType::forest);
				}
				if (tile->belowTile->GetTileType() == Tile::TileType::water) {
					tile->aboveTile->SetTileType(Tile::TileType::forest);
				}
				if (tile->rightTile->GetTileType() == Tile::TileType::water) {
					tile->aboveTile->SetTileType(Tile::TileType::forest);
				}
				if (tile->leftTile->GetTileType() == Tile::TileType::water) {
					tile->aboveTile->SetTileType(Tile::TileType::forest);
				}
			}
			groundSize++;
		}
	}
	//ground spawn next to more ground, add chance if more tiles
	//keep going until maxtiles
	//make some edges into sand
	//spawn some blobs of forest

}
void Tilemap::Clean() {
	SDL_DestroyTexture(tilemapTexture);
}
Tile::Tile() {

}
Tile::~Tile() {

}
void Tile::Init(int size, int xpos, int ypos) {
	tileRect.w = size;
	tileRect.h = size;
	tileRect.x = xpos;
	tileRect.y = ypos;

	//indexes i array
	arrX = xpos / size;
	arrY = ypos / size;

	tileType = TileType::water;
}