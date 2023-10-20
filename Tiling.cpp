#include "Tilemap.h"
#include <iostream>
#include <vector>
#include <random>
#include "Calculator.h"

Tilemap::Tilemap() {
	tileWidth = WIDTH / TILESIZE;
	tileHeight = HEIGHT / TILESIZE;
}
Tilemap::~Tilemap() {}

void Tilemap::Init(SDL_Renderer* renderer) {
	//tilemap
	tilemapSurface = SDL_LoadBMP("assets/Texture/watertiles.bmp");
	if (!tilemapSurface) {
		std::cerr << "Error loading BMP: " << SDL_GetError() << std::endl;
	}

	tilemapTexture = SDL_CreateTextureFromSurface(renderer, tilemapSurface);
	SDL_FreeSurface(tilemapSurface);
	if (!tilemapTexture) {
		std::cout << "notex" << std::endl;;
	}

	//decorsprites
	decormapSurface = SDL_LoadBMP("assets/Texture/decortiles.bmp");
	if (!decormapSurface) {
		std::cerr << "Error loading BMP: " << SDL_GetError() << std::endl;
	}

	decormapTexture = SDL_CreateTextureFromSurface(renderer, decormapSurface);
	SDL_FreeSurface(decormapSurface);
	if (!decormapTexture) {
		std::cout << "notex" << std::endl;;
	}

	//set coords and sizes
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			tilemap[x][y] = new Tile();
			tilemap[x][y]->Init(TILESIZE,x*TILESIZE,y*TILESIZE);
		}
	}
	//tile neighbors
	for (int x = 1; x < (tileWidth-1); x++) {
		for (int y = 1; y < (tileHeight-1); y++) {
			//top
			tilemap[x][y]->neighborTiles[0] = tilemap[x][y + 1];
			//right
			tilemap[x][y]->neighborTiles[1] = tilemap[x + 1][y];
			//bottom
			tilemap[x][y]->neighborTiles[2] = tilemap[x][y - 1];
			//left
			tilemap[x][y]->neighborTiles[3] = tilemap[x - 1][y];
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
	for (int x = 0; x < DECORSOURCETILESX; x++) {
		for (int y = 0; y < DECORSOURCETILESY; y++) {
			sourceTilesDecor[x][y].w = TILESIZE;
			sourceTilesDecor[x][y].h = TILESIZE;
			sourceTilesDecor[x][y].x = x * TILESIZE;
			sourceTilesDecor[x][y].y = y * TILESIZE;
		}
	}
}
void Tilemap::RenderTiles(SDL_Renderer* renderer) {
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			//ground
			Tilemap::GetTileMapCordsOfTileType(outx, outy, tilemap[x][y]->GetTileType());
			SDL_RenderCopy(renderer, tilemapTexture, &sourceTiles[*outx][*outy], &tilemap[x][y]->tileRect);

		}
	}
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
		
			//overlay
			if (tilemap[x][y]->overlayTile->GetTileType()!=Tile::empty) {

				for (size_t i = 0; i < tilemap[x][y]->overlayTile->treeHolder.size(); i++)
				{
					Tilemap::GetDecorMapCordsOfTileType(outx, outy, tilemap[x][y]->overlayTile->GetDecorType());
					SDL_RenderCopy(renderer, decormapTexture, &sourceTilesDecor[*outx][*outy], &tilemap[x][y]->overlayTile->treeHolder[i]);
				}
			}
		
		}
	}

}
void Tilemap::MakeIsland() {
	tilemap[tileWidth / 2][tileHeight / 2]->SetTileType(Tile::TileType::land_full);
	std::vector <Tile*> avaliabeSpawnTiles;

	while (groundSize < maxGroundSize) {
		bool dobreak = false; 

		//dont generate on edges
		for (int x = 3; x < (tileWidth-3); x++) {
			//break both loops on break
			if (dobreak)
				break;
			for (int y = 3; y < (tileHeight-3); y++) {
				Tile* tile = tilemap[x][y];

				//add avaliable neighbors of landtiles to vector
				if (tile->GetTileType() == Tile::land_full){
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

		//set new random first element of vector to be the new landtile
		avaliabeSpawnTiles[0]->SetTileType(Tile::land_full);
		landTiles.push_back(avaliabeSpawnTiles[0]);
		groundSize++;
		avaliabeSpawnTiles.clear(); //clean vector for next iteration
	}

	//ruletiling
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			tilemap[x][y]->SetTileTypeFromNeighbors();
		}
	}
		
	//make some edges into sand
	//spawn some blobs of forest
	SpawnForests(15, 50);
}
void Tilemap::SpawnForests(int startCount, int maxTileCount) {
	size_t my_size = landTiles.size();
	int currentCount = 0;
	std::vector<Tile*> forestTiles;
	std::vector<Tile*> availableSpawnTilesPine;
	std::vector<Tile*> availableSpawnTilesBirch;

	for (int i = 0; i < startCount; i++) {
		int r = Calculator::GetRandomIndex(0, my_size - 1);
		if (Calculator::GetRandomIndex(0, 1) > 0) {
			landTiles[r]->overlayTile->SetTileType(Tile::forest_pine);
		}
		else {
			landTiles[r]->overlayTile->SetTileType(Tile::forest_birch);
		}
		forestTiles.push_back(landTiles[r]);
		currentCount++;
	}

	while (currentCount < maxTileCount) {
		// Iterate through neighbors of all land tiles
		for (size_t i = 0; i < forestTiles.size(); i++) {
			for (Tile* tile : forestTiles[i]->neighborTiles) {
				if (tile->GetTileType() != Tile::land_full)
					continue;
				if (tile->overlayTile->GetTileType() != Tile::empty)
					continue;

				// Land tile is a forest, add neighbors to spawn points
				if (forestTiles[i]->overlayTile->GetTileType() == Tile::forest_pine) {
					availableSpawnTilesPine.push_back(tile);
				}
				else {
					availableSpawnTilesBirch.push_back(tile);
				}
			}
		}

		if (availableSpawnTilesBirch.empty() && availableSpawnTilesPine.empty())
			break;

		if (!availableSpawnTilesPine.empty()) {
			int random = Calculator::GetRandomIndex(0, availableSpawnTilesPine.size() - 1);
			availableSpawnTilesPine[random]->overlayTile->SetTileType(Tile::forest_pine);
			availableSpawnTilesPine[random]->overlayTile->SetDecorType(Tile::pine);
			availableSpawnTilesPine[random]->overlayTile->SetTreeDensity(5);
			availableSpawnTilesPine[random]->overlayTile->SpawnTrees();
			forestTiles.push_back(availableSpawnTilesPine[random]);
			currentCount++;
		}

		if (currentCount >= maxTileCount) {
			break;
		}

		if (!availableSpawnTilesBirch.empty()) {
			int random = Calculator::GetRandomIndex(0, availableSpawnTilesBirch.size() - 1);
			availableSpawnTilesBirch[random]->overlayTile->SetTileType(Tile::forest_birch);
			availableSpawnTilesBirch[random]->overlayTile->SetDecorType(Tile::birch);
			availableSpawnTilesBirch[random]->overlayTile->SetTreeDensity(5);
			availableSpawnTilesBirch[random]->overlayTile->SpawnTrees();
			forestTiles.push_back(availableSpawnTilesBirch[random]);
			currentCount++;
		}

		availableSpawnTilesBirch.clear();
		availableSpawnTilesPine.clear();
	}
}

void Tilemap::ClearIsland() {
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			tilemap[x][y]->SetTileType(Tile::water_full);
			tilemap[x][y]->overlayTile->SetTileType(Tile::TileType::empty);
			landTiles.clear();
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
Tile::Tile() {}
Tile::~Tile() {}
void Tile::Init(int size, int xpos, int ypos) {
	tileRect.w = size;
	tileRect.h = size;
	tileRect.x = xpos;
	tileRect.y = ypos;

	//indexes i array
	arrX = xpos / size;
	arrY = ypos / size;
	//default is water
	tileType = TileType::water_full;

	overlayTile = new Tile;
	overlayTile->tileRect.w = size;
	overlayTile->tileRect.h = size;
	overlayTile->tileRect.x = xpos;
	overlayTile->tileRect.y = ypos;
}
void Tile::SpawnTrees() {
	treeHolder.clear();
	treeHolder.resize(treeDensity);
	int x = 0, y = 0;
	for (size_t i = 0; i < treeDensity; i++)
	{
		//random tilecords
		x = Calculator::GetRandomIndex(0, Tilemap::TILESIZE);
		y = Calculator::GetRandomIndex(0, Tilemap::TILESIZE);

		treeHolder[i].w = 16;
		treeHolder[i].h = 16;
		treeHolder[i].x = x+tileRect.x;
		treeHolder[i].y = y+tileRect.y;
	}

}