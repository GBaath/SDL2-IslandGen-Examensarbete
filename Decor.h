#pragma once
#ifndef decor_h
#define decor_h
#include "SDL.h"
#include <vector>


class Tree {
public:

	int posY;
	SDL_Rect rect;
	Uint8 colorShade;

	int textureMapCordsX, textureMapCordsY;
};
class Moutain {
public:
	void Init(int size, int posX, int posY) {
		if (size < 2)
			size = 2;
		if (size > 7)
			size = 7;
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
		leftend.y = 0;
		lefttrans.x = 32;
		lefttrans.y = 0;
		middle.x = 64;
		middle.y = 0;
		righttrans.x = 96;
		righttrans.y = 0;
		rightend.x = 128;
		rightend.y = 0;


		mountainSource.clear();
		mountainRects.clear();
		mountainRects.resize(size);
		mountainSource.push_back(leftend);
		if (size <= 2) {
			mountainSource.push_back(rightend);
		}
		else if (size == 3) {
			mountainSource.push_back(middle);
			mountainSource.push_back(rightend);
		}
		else if (size == 4) {
			mountainSource.push_back(lefttrans);
			mountainSource.push_back(righttrans);
			mountainSource.push_back(rightend);
		}
		else {
			mountainSource.push_back(lefttrans);
			for (size_t i = 0; i < size - 4; i++)
			{
				mountainSource.push_back(middle);
			}
			mountainSource.push_back(righttrans);
			mountainSource.push_back(rightend);
		}

		for (size_t i = 0; i < size; i++)
		{
			mountainRects[i].h = 64;
			mountainRects[i].w = 32;
			mountainRects[i].y = posY;
			mountainRects[i].x = posX+32*i;
		}
	}

	int posY;
	Uint8 colorShade;
	std::vector<SDL_Rect> mountainSource;
	std::vector<SDL_Rect> mountainRects;

private:

	SDL_Rect leftend, lefttrans, middle, righttrans, rightend;
};
#endif "decor.h"