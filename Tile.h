#pragma once
#ifndef tile_h
#define tile_h
#include <iostream>
#include <vector>
#include "Tilemap.h"
#include "Decor.h";




class Tile {
public:
	enum TileType {empty, water_full,land_full, 
		toprightLand, topLand,topleftLand,
		leftLand,rightLand, leftrightLand, topbottomLand,
		bottomleftLand,bottomLand,bottomrightLand,
		topWater,bottomWater,
		rightWater,leftWater,
		pond, 
		riverDeltaRight, riverDeltaTop, riverDeltaLeft, riverDeltaBot,
		riverHorizontal, riverVertical,
		riverTopRight, riverRightBot, riverBotLeft, riverLeftTop,
		riverEndLeft, riverEndRight, riverEndTop, riverEndBot,
		forest_pine, forest_birch
	};
	enum DecorType {
		none, pine, birch
	};


	Tile();
	~Tile();
	
	void Init(int size, int xpos, int ypos);
	void SetTileType(TileType type) { tileType = type; }
	void SetDecorType(DecorType type) { decorType = type; }
	void SetTileTypeFromNeighbors() {


		if (GetTileType() != water_full)
			return;
		bool top= false, right= false, bot = false, left = false;

		if (neighborTiles[0] != nullptr) 
			top = neighborTiles[0]->GetTileType() == land_full;
		if (neighborTiles[1] != nullptr)
			right = neighborTiles[1]->GetTileType() == land_full;
		if (neighborTiles[2] != nullptr)
			bot = neighborTiles[2]->GetTileType() == land_full;
		if (neighborTiles[3] != nullptr)
			left = neighborTiles[3]->GetTileType() == land_full;

		

		//4
		if (!top && !right && !bot && !left) {
			SetTileType(water_full);
		}
		else if (top && right && bot && left) {
			SetTileType(pond);
		}
		//3
		else if (top && !right && !bot && !left) {
			SetTileType(bottomLand);
		}
		else if (!top && right && !bot && !left) {
			SetTileType(rightLand);
		}
		else if (!top && !right && bot && !left) {
			SetTileType(topLand);
		}
		else if (!top && !right && !bot && left) {
			SetTileType(leftLand);
		}
		//2
		else if (top && right && !bot && !left) {
			SetTileType(bottomrightLand);
		}
		else if (top && !right && bot && !left) {
			SetTileType(topbottomLand);
		}
		else if (top && !right && !bot && left) {
			SetTileType(bottomleftLand);
		}
		else if (!top && right && bot && !left) {
			SetTileType(toprightLand);
		}
		else if (top && right && !bot && !left) {
			SetTileType(rightLand);
		}
		else if (!top && !right && bot && left) {
			SetTileType(topleftLand);
		}
		else if (!top && right && !bot && left) {
			SetTileType(leftrightLand);
		}
		//1
		else if (top && right && bot && !left) {
			SetTileType(leftWater);
		}
		else if (!top && right && bot && left) {
			SetTileType(bottomWater);
		}
		else if (top && !right && bot && left) {
			SetTileType(rightWater);
		}
		else if (top && right && !bot && left) {
			SetTileType(topWater);
		}
		

	}
	void SpawnTrees(int density,DecorType type);

	bool HasAsNeighbor(Tile* tile) {
		for (Tile* tile : neighborTiles) {
			if (tile == tile)
				return true;
		}
		return false;
	}
	int GetTreeDensity() { return treeDensity; }
	

	TileType GetTileType() { return tileType; }
	DecorType GetDecorType() { return  decorType; } 
	Tile* neighborTiles[4]{nullptr,nullptr,nullptr,nullptr};
	Tile* overlayTile = nullptr;
	Tile* spawnedFrom = nullptr;
	SDL_Rect tileRect;
	std::vector <Tree> treeHolder;
private:
	int arrX, arrY;
	int treeDensity = 0;

	TileType tileType;
	DecorType decorType;
};
#endif "tile.h"