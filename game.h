#ifndef game_h
#define game_h

#include "SDL.h"
#include <stdio.h>
#include <iostream>
#include "SDL_image.h"
#include "tilemap.h"

class Game
{
public:
	Game();
	~Game();
	void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void HandleEvents();
	void Update();
	void Render();
	void Clean();


	bool GameRunning() { return isRunning; }

private:
	void CancelLoad();


	struct FrameData {
		int framecnt = 0;
		const int targetFPS = 30;
		const Uint32 frameDelay = 1000 / targetFPS; 
		Uint32 frameStart = 0;
		Uint32 frameTime = 0;
	};
	FrameData fdata;



	bool isRunning = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	Tilemap* tilemap;


};


#endif "game_h"

