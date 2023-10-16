#include "Tilemap.h"
#include <iostream>
#include <vector>
#include <random>

Tilemap::Tilemap() {
	tileWidth = WIDTH / TILESIZE;
	tileHeight = HEIGHT / TILESIZE;
}
Tilemap::~Tilemap() {

}
void Tilemap::Init(SDL_Renderer* renderer) {
	tilemapSurface = SDL_LoadBMP("assets/Texture/watertiles.bmp");
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

	//read sourceimage and set values
	for (int x = 0; x < SOURCETILESX; x++) {
		for (int y = 0; y < SOURCETILESY; y++) {
			sourceTiles[x][y].w = TILESIZE;
			sourceTiles[x][y].h = TILESIZE;
			sourceTiles[x][y].x = x * TILESIZE;
			sourceTiles[x][y].y = y * TILESIZE;
		}
	}

}
void Tilemap::RenderTiles(SDL_Renderer* renderer) {

	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			Tilemap::GetTileMapCordsOfTileType(outx, outy, tilemap[x][y]->GetTileType());
			SDL_RenderCopy(renderer, tilemapTexture, &sourceTiles[*outx][*outy], &tilemap[x][y]->tileRect);
		}
	}
}
void Tilemap::MakeIsland() {

	tilemap[tileWidth / 2][tileHeight / 2]->SetTileType(Tile::TileType::forest_full);

	//dont generate on edges
	std::vector <Tile*> avaliabeSpawnTiles;
	while (groundSize < maxGroundSize) {

		bool dobreak = false; 
		for (int x = 2; x < (tileWidth-2); x++) {
			//break both loops on break
			if (dobreak)
				break;
			for (int y = 2; y < (tileHeight-2); y++) {


				Tile* tile = tilemap[x][y];

				//top
				tile->neighborTiles[0] = tilemap[x][y+1];
				//right
				tile->neighborTiles[1] = tilemap[x+1][y];
				//bottom
				tile->neighborTiles[2] = tilemap[x][y-1];
				//left
				tile->neighborTiles[3] = tilemap[x-1][y];

				//add avaliable neighbors of landtiles to vector
				if (tile->GetTileType() == Tile::forest_full){
					for (Tile* tile : tile->neighborTiles) {
						if (tile->GetTileType() == Tile::water_full)
							avaliabeSpawnTiles.push_back(tile);
					}
				}

			}
		}
		//new random tile
		std::random_device rd; // Use a random device to seed the generator
		std::mt19937 gen(rd()); // Create a Mersenne Twister random number generator
		std::shuffle(avaliabeSpawnTiles.begin(), avaliabeSpawnTiles.end(), gen);
		avaliabeSpawnTiles[0]->SetTileType(Tile::forest_full);
		groundSize++;

		avaliabeSpawnTiles.clear();
	}

	//ruletiling
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			std::cout << y << x << std::endl;
			tilemap[x][y]->SetTileTypeFromNeighbors();
		}
	}
		
	//ground spawn next to more ground, add chance if more tiles
	//keep going until maxtiles
	//make some edges into sand
	//spawn some blobs of forest

}
void Tilemap::ClearIsland() {
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			tilemap[x][y]->SetTileType(Tile::water_full);
			groundSize = 0;
		}
	}
}
bool CheckTileAvaliable(Tile* tile) {
	if (tile->GetTileType() == Tile::water_full)
		return true;
	else
		return false;
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

	tileType = TileType::water_full;
}