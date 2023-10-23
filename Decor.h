#pragma once
#ifndef decor_h
#define decor_h
#include "SDL.h"
#include <vector>


class Tree {
public:
	SDL_Rect rect;
	Uint8 colorShade;

	int textureMapCordsX, textureMapCordsY;
};
class MoutainPart {
public:
	MoutainPart(int size) {
		if (size < 2)
			size = 2;
		leftend.h = 64;
		leftend.w = 32;
		lefttrans.h = 64;
		lefttrans.w = 32;
		middle.h = 64;
		middle.w = 32;
		righttrans.h = 64;
		righttrans.w = 32;
		rightend.h = 64;
		rightend.w = 32;

		leftend.x = 0;
		leftend.y = 64;
		lefttrans.x = 32;
		lefttrans.y = 64;
		middle.x = 64;
		middle.y = 64;
		righttrans.x = 96;
		righttrans.y = 64;
		rightend.x = 128;
		rightend.y = 64;


		mountain.clear();
		mountain.push_back(leftend);
		if (size <= 2) {
			mountain.push_back(rightend);
		}
		else if (size == 3) {
			mountain.push_back(middle);
			mountain.push_back(rightend);
		}
		else if (size == 4) {
			mountain.push_back(lefttrans);
			mountain.push_back(righttrans);
			mountain.push_back(rightend);
		}
		else{
			mountain.push_back(lefttrans);
			for (size_t i = 0; i < size-5; i++)
			{
				mountain.push_back(middle);
			}
			mountain.push_back(righttrans);
			mountain.push_back(rightend);
		}
	}


	SDL_Rect rect;
	Uint8 colorShade;
	std::vector<SDL_Rect> mountain;

private:
	SDL_Rect leftend, lefttrans, middle, righttrans, rightend;

};
#endif "decor.h"