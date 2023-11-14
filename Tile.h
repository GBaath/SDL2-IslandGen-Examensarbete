#pragma once
#ifndef tile_h
#define tile_h
#include <iostream>
#include <vector>
#include "Tilemap.h"
#include "Decor.h";
#include "Calculator.h"




class Tile {
public:
	enum TileType {empty, water_full,land_full, 
		toprightLand, topLand,topleftLand,
		leftLand,rightLand, leftrightLand, topbottomLand,
		bottomleftLand,bottomLand,bottomrightLand,
		topWater,bottomWater,
		rightWater,leftWater,
		pond, riverBase, riverDeltaBase,
		riverDeltaRight, riverDeltaTop, riverDeltaLeft, riverDeltaBot,
		riverDeltaEdgeRight, riverDeltaEdgeTop, riverDeltaEdgeLeft, riverDeltaEdgeBot,
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

		//rivers with landconnection on side
		if (neighborTiles[0] != nullptr)
			if (neighborTiles[0]->GetTileType() == land_full || neighborTiles[0]->GetTileType() == riverRightBot || (neighborTiles[0]->GetTileType() == riverHorizontal 
				|| (neighborTiles[0]->GetTileType() == riverBotLeft || (neighborTiles[0]->GetTileType() == riverDeltaRight || (neighborTiles[0]->GetTileType() == riverDeltaLeft)))))
					bot = true;

		if (neighborTiles[1] != nullptr)
			if (neighborTiles[1]->GetTileType() == land_full || neighborTiles[1]->GetTileType() == riverVertical || (neighborTiles[1]->GetTileType() == riverBotLeft
				|| (neighborTiles[1]->GetTileType() == riverLeftTop || (neighborTiles[1]->GetTileType() == riverDeltaTop || (neighborTiles[1]->GetTileType() == riverDeltaBot)))))
					right = true;

		if (neighborTiles[2] != nullptr)
			if (neighborTiles[2]->GetTileType() == land_full || neighborTiles[2]->GetTileType() == riverTopRight || (neighborTiles[2]->GetTileType() == riverHorizontal
				|| (neighborTiles[2]->GetTileType() == riverLeftTop || (neighborTiles[2]->GetTileType() == riverDeltaRight || (neighborTiles[2]->GetTileType() == riverDeltaLeft)))))
					top = true;
		if (neighborTiles[3] != nullptr)
			if (neighborTiles[3]->GetTileType() == land_full || neighborTiles[3]->GetTileType() == riverVertical || (neighborTiles[3]->GetTileType() == riverRightBot
				|| (neighborTiles[3]->GetTileType() == riverTopRight || (neighborTiles[3]->GetTileType() == riverDeltaTop || (neighborTiles[3]->GetTileType() == riverDeltaBot)))))
				left = true;


		

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

	void SetRandomRiverConnection(int dir, int* dirOut,int illegalDir, std::vector <Tile*> riverTiles, bool isDelta = false) {
		//dir 0 1 2 3 = up, right, down, left
		if (dir < 0)
			dir = 0;
		if (dir > 3)
			dir = 3;

		std::vector <int> illdirs = {-1,-1,-1,-1, illegalDir};

		//is neighbor a river
		for (size_t i = 0; i < 4; i++)
		{ 
			if (std::find(riverTiles.begin(), riverTiles.end(), neighborTiles[i]) != riverTiles.end()) {
				illdirs[i] = i;
			}
		}

		int r = Calculator::GetRandomIndex(0, 2);

		if (isDelta) {
			switch (dir)
			{
			case 0:
				SetTileType(riverDeltaBot);
				break;
			case 1:
				SetTileType(riverDeltaLeft);
				break;
			case 2:
				SetTileType(riverDeltaTop);
				break;
			case 3:
				SetTileType(riverDeltaRight);
				break;
			default:
				SetTileType(empty);
				break;
			}
		}
		
		//please dont look here
		//spawndirection data for where to connect next rivertile
		int consecfails = -1;
		do {
			consecfails++;
			switch (dir)
			{
			case 0:
				switch (r)
				{
				case 0:
					for(int i : illdirs)
					{
						*dirOut = 1;
						if(i==*dirOut)
							continue;
						else {
							SetTileType(riverTopRight);
							return;
						}
					}
					break;
				case 1:
					for(int i : illdirs)
					{
						*dirOut = 2;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverVertical);
							return;

						}
					}
					break;
				case 2:
					for(int i : illdirs)
					{
						*dirOut = 3;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverLeftTop);
							return;

						}
					}
					break;
				}			
			case 1:
				switch (r)
				{
				case 0:
					for(int i : illdirs)
					{
						*dirOut = 0;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverTopRight);
							return;

						}
					}
					break;
				case 1:
					for(int i : illdirs)
					{
						*dirOut = 3;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverHorizontal);
							return;

						}
					}
				case 2:
					for(int i : illdirs)
					{
						*dirOut = 2;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverRightBot);
							return;

						}
					}

					break;
				}				
			case 2:
				switch (r)
				{
				case 0:
					for(int i : illdirs)
					{
						*dirOut = 0;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverVertical);
							return;

						}
					}

					break;
				case 1:
					for(int i : illdirs)
					{
						*dirOut = 1;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverRightBot);
							return;

						}
					}

					break;
				case 2:
					for(int i : illdirs)
					{
						*dirOut = 3;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverBotLeft);
							return;

						}
					}
					break;
				}
			case 3:
				switch (r)
				{
				case 0:
					for(int i : illdirs)
					{
						*dirOut = 0;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverLeftTop);
							return;

						}
					}
					break;
				case 1:
					for(int i : illdirs)
					{
						*dirOut = 1;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverHorizontal);
							return;

						}
					}
					break;
				case 2:
					for(int i : illdirs)
					{
						*dirOut = 2;
						if(i==*dirOut)
							continue;
						else{
							SetTileType(riverBotLeft);
							return;

						}
					}
					break;
				}
				
			}
		}
		while (consecfails < 15);


		CloseThisRiverTile(dir);
		return;

		//after consecfails
		switch (dir) {
		case 0:
			SetTileType(riverEndTop);
			return;
		case 3:
			SetTileType(riverEndRight);
			return;
		case 2:
			SetTileType(riverEndBot);
			return;
		case 1:
			SetTileType(riverEndLeft);
			return;
		}

	
	}

	//bugged, spawning on existing river
	//TODO remake tilemap and spawn edge on top
	void CloseThisRiverTile(int dir) 
	{
		if (dir < 0)
			dir = 0;
		if (dir > 3)
			dir = 3;

		//make inte reiver delta if spawn into water
		if (neighborTiles[dir]->GetTileType() == water_full) {
			switch (dir) {
			case 0:
				SetTileType(riverDeltaBot);
				break;
			case 1:
				SetTileType(riverDeltaLeft);
				break;
			case 2:
				SetTileType(riverDeltaTop);
				break;
			case 3:
				SetTileType(riverDeltaRight);
				break;

			default:
				break;
			}
			return;
		}

		switch (dir) {
		case 0:
			SetTileType(riverEndTop);
			break;
		case 1:
			SetTileType(riverEndRight);
			break;
		case 2:
			SetTileType(riverEndBot);
			break;
		case 3:
			SetTileType(riverEndLeft);
			break;

		default: 
			break;
		}
	}
	void SetRiverTileFromNeighbors(bool isDelta) {


		bool top = false, right = false, bot = false, left = false;

		//rivers with landconnection on side
		switch (neighborTiles[0]->GetTileType()) {
		case riverDeltaBase:
		case riverBase:
		case riverRightBot:
		case riverVertical:
		case riverBotLeft:
		case riverDeltaTop:
			top = true;
			break;

		default:
			top = false;
			break;
		}
		switch (neighborTiles[1]->GetTileType()) {
		case riverDeltaBase:
		case riverBase:
		case riverHorizontal:
		case riverBotLeft:
		case riverLeftTop:
		case riverDeltaRight:
			right = true;
			break;

		default:
			right = false;
			break;
		}
		switch (neighborTiles[2]->GetTileType()) {
		case riverDeltaBase:
		case riverBase:
		case riverTopRight:
		case riverVertical:
		case riverLeftTop:
		case riverDeltaBot:
			bot = true;
			break;

		default:
			bot = false;
			break;
		}
		switch (neighborTiles[3]->GetTileType()) {
		case riverDeltaBase:
		case riverBase:
		case riverHorizontal:
		case riverRightBot:
		case riverTopRight:
		case riverDeltaLeft:
			left = true;
			break;

		default:
			left = false;
			break;
		}

				
		if (isDelta) {
			if (top)
				SetTileType(riverDeltaBot);
			else if (right)
				SetTileType(riverDeltaLeft);
			else if (bot)
				SetTileType(riverDeltaTop);
			else if (left)
				SetTileType(riverDeltaRight);
				
			return;
		}
		
		if (top && right)
			SetTileType(riverTopRight);
		else if (top && bot)
			SetTileType(riverVertical);
		else if (top && left)
			SetTileType(riverLeftTop);
		else if (right && bot)
			SetTileType(riverRightBot);
		else if (right && left)
			SetTileType(riverHorizontal);
		else if (bot && left)
			SetTileType(riverBotLeft);
	}
	Tile* GetNearestLandNeighborInDirection(int dir) {
		if (dir < 0)
			dir = 0;
		if (dir > 3)
			dir = 3;

		//loop thru neighbors
		if (neighborTiles[dir]->GetTileType() == land_full)
			return neighborTiles[dir];
		else
			return neighborTiles[dir]->GetNearestLandNeighborInDirection(dir);
	}

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