#pragma once
#ifndef tile_h
#define tile_h
class Tile {
public:
	enum TileType {water_full,forest_full, 
		toprightLand, topLand,topleftLand,
		leftLand,rightLand,
		bottomleftLand,bottomLand,bottomrightLand,
		topWater,bottomWater,
		rightWater,leftWater,
		pond};

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
	void SetTileTypeFromNeighbors() {


		if (GetTileType() != water_full)
			return;
		bool top= false, right= false, bot = false, left = false;

		if(neighborTiles[0] != nullptr)
			top = neighborTiles[0]->GetTileType() == forest_full;
		if (neighborTiles[1] != nullptr)
			right = neighborTiles[1]->GetTileType() == forest_full;
		if (neighborTiles[2] != nullptr)
			bot = neighborTiles[2]->GetTileType() == forest_full;
		if (neighborTiles[3] != nullptr)
			left = neighborTiles[3]->GetTileType() == forest_full;


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
		/*else if (top && !right && bot && !left) { //no sprite
			SetTileType(topLand);
		}*/
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
		/*else if (!top && right && !bot && left) { //no sprite
			SetTileType(topleftLand);
		}*/
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

	Tile* neighborTiles[4]{nullptr,nullptr,nullptr,nullptr};


	SDL_Rect tileRect;
private:
	TileType tileType;
	int arrX, arrY;

};
#endif "tile.h"