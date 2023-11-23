#include "Tilemap.h"
#include <iostream>
#include <vector>
#include <random>
#include "Calculator.h"
#include <algorithm>


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

	mountainSurface = SDL_LoadBMP("assets/Texture/mountain.bmp");
	if (!mountainSurface) {
		std::cerr << "Error loading BMP: " << SDL_GetError() << std::endl;
	}
	mountainTexture = SDL_CreateTextureFromSurface(renderer, mountainSurface);
	if (!mountainTexture) {
		std::cout << "notex" << std::endl;;
	}
	SDL_FreeSurface(mountainSurface);

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
			tilemap[x][y]->neighborTiles[0] = tilemap[x][y - 1];
			//right
			tilemap[x][y]->neighborTiles[1] = tilemap[x + 1][y];
			//bottom
			tilemap[x][y]->neighborTiles[2] = tilemap[x][y + 1];
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
	for  (int i = 0; i < MOUNTAINSOURCETILES; i++)
	{
		sourceTilesMtn[i].w = TILESIZE;
		sourceTilesMtn[i].h = TILESIZE * 2;
		sourceTilesMtn[i].x = i * TILESIZE;
		sourceTilesMtn[i].y = 0;
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
	//render decor
	for (DecorBase* element : renderingList)
	{
		if (dynamic_cast<Tree*>(element)) {
			Tree* tree = dynamic_cast<Tree*>(element);
			SDL_SetTextureColorMod(decormapTexture, tree->colorShade, tree->colorShade, tree->colorShade);
			SDL_RenderCopy(renderer, decormapTexture, &sourceTilesDecor[tree->textureMapCordsX][tree->textureMapCordsY], &tree->rect);
			//std::cout << tree->yPos << "tree" << std::endl;
		}
		else if (dynamic_cast<Mountain*>(element)) {
			Mountain* mountain = dynamic_cast<Mountain*>(element);
			int j = mountain->sourceListIndex;

			for (size_t i = 0; i < mountains[j].mountainRects.size(); i++)
			{ 
				SDL_RenderCopy(renderer, mountainTexture, &mountains[j].mountainSource[i], &mountains[j].mountainRects[i]);
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
				Tile* sourcetile = tilemap[x][y];

				//add avaliable neighbors of landtiles to vector
				if (sourcetile->GetTileType() == Tile::land_full){
					for (Tile* tile : sourcetile->neighborTiles) {
						if (tile->GetTileType() == Tile::water_full) {
							avaliabeSpawnTiles.push_back(tile);
							tile->spawnedFrom = sourcetile;
						}
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

	//more water here
	BonusWater(2,6,5,1);

		
	//ruletiling
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			tilemap[x][y]->SetTileTypeFromNeighbors();
		}
	}
	SpawnForests(10, 60);
	SpawnMountains(4);


	//add all trees to renderinglist
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = tileHeight - 1; y >= 0; y--) {
			if (tilemap[x][y]->overlayTile->GetTileType() != Tile::empty) {

				for (size_t i = 0; i < tilemap[x][y]->overlayTile->treeHolder.size(); i++)
				{
					Tree* tree = &tilemap[x][y]->overlayTile->treeHolder[i];
					tree->yPos = tree->rect.y+24; //magic nr offset

					renderingList.push_back(tree);
				}
			}
		}
	}
	//add mountain to renderinglist
	for (int j = 0; j < mountains.size(); j++)
	{
		for (int i = 0; i < mountains[j].mountainRects.size(); i++)
		{
			mountains[j].yPos = mountains[j].mountainRects[0].y + 64; //tilesizeoffset
			mountains[j].sourceListIndex = j;
			renderingList.push_back(&mountains[j]);
		}
	}

	std::sort(renderingList.begin(), renderingList.end(), [](const DecorBase* a, const DecorBase* b) {
		return a->yPos < b->yPos;
		});
}
void Tilemap::BonusWater(int lakeTilesSources, int bonusLakeTiles, int riverCount, int riverSources) {
	std::vector <Tile*> avaliabeSpawnTiles;
	std::vector <Tile*> lakeTiles;

	//iterate thru all tiles check for tiles with only land neighbor, make to source
	while (lakeTilesSources > 0) {
		for (int x = 3; x < (tileWidth-3); x++) {
			for (int y = 3; y < tileHeight-3; y++) {
				Tile* sourcetile = tilemap[x][y];
				bool compatable = true;

				for (Tile* tile : sourcetile->neighborTiles) {
					if (tile->GetTileType() != Tile::land_full)
						continue;
					for (Tile* ntile : tile->neighborTiles) {
						//no check for source
						if (ntile == tile)
							continue;
						//all others are land
						if (ntile->GetTileType() == Tile::water_full)
							compatable = false;
					}
					if (compatable) {
						avaliabeSpawnTiles.push_back(tile);
					}
					compatable = true;
				}
			}
		}
		int random = Calculator::GetRandomIndex(0, (avaliabeSpawnTiles.size()-1));

		avaliabeSpawnTiles[random]->SetTileType(Tile::water_full);
		lakeTiles.push_back(avaliabeSpawnTiles[random]);
		lakeTilesSources--;
		avaliabeSpawnTiles.clear();
	}

	while (bonusLakeTiles > 0) {
		//expand random sources until == bonuslaketiles
		//iterate through lakesources and check neighboring tiles are land
		bool compatable = true;
		for (Tile* tile : lakeTiles) {	
			compatable = true;
			//ntile next to source
			for (Tile* ntile : tile->neighborTiles) {
				compatable = true;
				//iterate neighbors of neighbor
				for (Tile* nntile : ntile->neighborTiles) {
					//skip the tile that sent the call
					if (nntile == ntile)
						continue;
					//neighbortile is allready in vector
					if (std::find(lakeTiles.begin(), lakeTiles.end(), nntile) != lakeTiles.end())
						continue;
					//all others are land
					if (nntile->GetTileType() == Tile::water_full)
						compatable = false;
				}
				if (compatable) {
					avaliabeSpawnTiles.push_back(ntile);
				}
			}
		}
		//select random avaliable add make water
		if (!avaliabeSpawnTiles.empty()) {
			int random = Calculator::GetRandomIndex(0, (avaliabeSpawnTiles.size() - 1));

			avaliabeSpawnTiles[random]->SetTileType(Tile::water_full);
			lakeTiles.push_back(avaliabeSpawnTiles[random]);
			avaliabeSpawnTiles.clear();

			bonusLakeTiles--;
		}
		else
			break;
	}
	
	std::vector <Tile*> riverTiles;
	int riverlength = riverCount;

	for (size_t i = 0; i < riverSources; i++)
	{
		riverCount = riverlength;

		//spawn river in random lake chose direction, randomwalk
		Tile* riverStart = lakeTiles[Calculator::GetRandomIndex(0, (lakeTiles.size() - 1))];
		riverTiles.push_back(riverStart);
		int randomDir = Calculator::GetRandomIndex(0, 3);
		int startDir = randomDir;

		Tile* riverdelta = riverStart->GetNearestLandNeighborInDirection(randomDir);
		Tile::TileType startDelta = Tile::riverDeltaBase;
		Tile::TileType startRiver = Tile::riverBase;
		//holders
		int dirHolder = 0;
		int* nextDir = &dirHolder;
		int prevDir = randomDir;

		//set startingSpriteDir
		switch (randomDir) {
		case 0:
			startDelta = Tile::riverDeltaBot;
			riverdelta->neighborTiles[randomDir]->SetRandomRiverConnection(2, nextDir,0, riverTiles);
			break;
		case 1:
			startDelta = Tile::riverDeltaLeft;
			riverdelta->neighborTiles[randomDir]->SetRandomRiverConnection(3, nextDir,1, riverTiles);
			break;
		case 2:
			startDelta = Tile::riverDeltaTop;
			riverdelta->neighborTiles[randomDir]->SetRandomRiverConnection(0, nextDir,2, riverTiles);
			break;
		case 3:
			startDelta = Tile::riverDeltaRight;
			riverdelta->neighborTiles[randomDir]->SetRandomRiverConnection(1, nextDir,3, riverTiles);
			break;
		}

		//start
		riverdelta->SetTileType(startDelta);
		riverTiles.push_back(riverdelta);
		riverTiles.push_back(riverdelta->neighborTiles[randomDir]);

		while (riverCount > 0){
			Tile* _new = riverTiles[riverTiles.size() - 1]->neighborTiles[*nextDir];
			if (_new->GetTileType() != Tile::water_full) {

				prevDir = *nextDir;
				_new->SetRandomRiverConnection((*nextDir+2)%4, nextDir, (startDir+2)%4, riverTiles);
				riverTiles.push_back(_new);
			}


			riverCount--;
		}
		riverTiles[riverTiles.size() - 1]->CloseThisRiverTile(((prevDir + 2) % 4));

	}

	
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
			availableSpawnTilesPine[random]->overlayTile->SpawnTrees(Calculator::GetRandomIndex(3,8), Tile::pine);
			forestTiles.push_back(availableSpawnTilesPine[random]);
			currentCount++;
		}

		if (currentCount >= maxTileCount) {
			break;
		}

		if (!availableSpawnTilesBirch.empty()) {
			int random = Calculator::GetRandomIndex(0, availableSpawnTilesBirch.size() - 1);
			availableSpawnTilesBirch[random]->overlayTile->SetTileType(Tile::forest_birch);
			availableSpawnTilesBirch[random]->overlayTile->SpawnTrees(Calculator::GetRandomIndex(3, 8),Tile::birch);
			forestTiles.push_back(availableSpawnTilesBirch[random]);
			currentCount++;
		}

		availableSpawnTilesBirch.clear();
		availableSpawnTilesPine.clear();
	}
}
void Tilemap::SpawnMountains(int count) {
	mountains.clear();
	int newSize = (Calculator::GetRandomIndex(1, count));
	mountains.resize(newSize);
	for (size_t i = 0; i < mountains.size(); i++)
	{
		int posx, posy;
		int index = Calculator::GetRandomIndex(0, landTiles.size());
		posx = landTiles[index]->tileRect.x;
		posy = landTiles[index]->tileRect.y;

		int size = Calculator::GetRandomIndex(0, 7);
		
		mountains[i].Init(size, posx,posy);
		
	}
}


void Tilemap::ClearIsland() {
	for (int x = 0; x < (tileWidth); x++) {
		for (int y = 0; y < (tileHeight); y++) {
			tilemap[x][y]->SetTileType(Tile::water_full);
			tilemap[x][y]->overlayTile->SetTileType(Tile::TileType::empty);
		}
	}
	renderingList.clear();
	renderingList.resize(0);
	landTiles.clear();
	mountains.clear();
	groundSize = 0;
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
void Tile::SpawnTrees(int density,DecorType type) {

	SetDecorType(type);
	treeDensity = density;
	treeHolder.clear();
	treeHolder.resize(treeDensity);
	int x = 0, y = 0;
	for (size_t i = 0; i < treeDensity; i++)
	{
		//random tilecords
		x = Calculator::GetRandomIndex(-Tilemap::TILESIZE/2, Tilemap::TILESIZE/2);
		y = Calculator::GetRandomIndex(-Tilemap::TILESIZE/2, Tilemap::TILESIZE/2);

		treeHolder[i].rect.w = 16;
		treeHolder[i].rect.h = 16;
		treeHolder[i].rect.x = x+tileRect.x;
		treeHolder[i].rect.y = y+tileRect.y;

		treeHolder[i].colorShade = Calculator::GetRandomIndex(200, 255);

		Tilemap::GetDecorMapCordsOfTileType(&treeHolder[i].textureMapCordsX, &treeHolder[i].textureMapCordsY, type);
	}
}