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
	std::vector <Tile*> avaliabeSpawnTiles;
	while (groundSize < maxGroundSize) {

		bool dobreak = false; 
		for (int x = 1; x < (tileWidth-1); x++) {
			//break both loops on break
			if (dobreak)
				break;
			for (int y = 1; y < (tileHeight-1); y++) {


				Tile* tile = tilemap[x][y];


				tile->neighborTiles[0] = tilemap[x][y+1];
				tile->neighborTiles[1] = tilemap[x+1][y];
				tile->neighborTiles[2] = tilemap[x][y-1];
				tile->neighborTiles[3] = tilemap[x-1][y];

				//add avaliable neighbors of landtiles to vector
				if (tile->GetTileType() == Tile::forest){
					for (Tile* tile : tile->neighborTiles) {
						if (tile->GetTileType() == Tile::water)
							avaliabeSpawnTiles.push_back(tile);
					}
				}

			}
		}
		//new random tile
		std::random_device rd; // Use a random device to seed the generator
		std::mt19937 gen(rd()); // Create a Mersenne Twister random number generator
		std::shuffle(avaliabeSpawnTiles.begin(), avaliabeSpawnTiles.end(), gen); // Define a distribution for indices
		avaliabeSpawnTiles[0]->SetTileType(Tile::forest);
		groundSize++;

		avaliabeSpawnTiles.clear();
	}
		
	//ground spawn next to more ground, add chance if more tiles
	//keep going until maxtiles
	//make some edges into sand
	//spawn some blobs of forest

}
void Tilemap::ClearIsland() {
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			tilemap[x][y]->SetTileType(Tile::water);
			groundSize = 0;
		}
	}
}
bool CheckTileAvaliable(Tile* tile) {
	if (tile->GetTileType() == Tile::water)
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

	tileType = TileType::water;
}