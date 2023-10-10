#ifndef game_h
#define game_h

#include "SDL.h"
#include <stdio.h>
#include <iostream>
class Game
{
public:
	Game();
	~Game();
	void Init(const char* title, int xpos, int ypos, int wight, int height, bool fullscreen);
	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool GameRunning() { return isRunning; }

private:
	struct FrameData {
		int framecnt;
		const int targetFPS = 60;
		const Uint32 frameDelay = 1000 / targetFPS; 
		Uint32 frameStart;
		Uint32 frameTime;
	};
	FrameData fdata;

	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

};


#endif "game_h"

