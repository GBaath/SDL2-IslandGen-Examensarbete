#include "game.h"
#include "tilemap.h"
#include <stdlib.h>


Game::Game() {

}
Game::~Game() {

}
void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) 
{
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		std::cout << "initialized" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "window created" << std::endl;	
		}
		else {
			CancelLoad();
			return;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "renderer created" << std::endl;
		}
		else {
			CancelLoad();
			return;
		}

		tilemap = new Tilemap();
		tilemap->Init(renderer);

		isRunning = true;
	}
	else {
		CancelLoad();
	}
}
void Game::CancelLoad() {
	isRunning = false;
}
void Game::HandleEvents() 
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type){
		case SDL_QUIT:
			isRunning = false;
			break;
			
		case SDL_KEYDOWN: {
			if(event.key.keysym.sym == SDLK_SPACE)
				tilemap->MakeIsland();
			break;
		}
		default:
			break;
	}
}
void Game::Update() 
{
	fdata.frameStart = SDL_GetTicks();
	fdata.framecnt++;
	std::cout << fdata.framecnt << std::endl;

	//stuff
	
	fdata.frameTime = SDL_GetTicks() - fdata.frameStart;

	if (fdata.frameTime < fdata.frameDelay) {
		SDL_Delay(fdata.frameDelay - fdata.frameTime);
	}
}
void Game::Render() 
{
	SDL_RenderClear(renderer);
	
	//stuff
	tilemap->RenderTiles(renderer);

	SDL_RenderPresent(renderer);
}
void Game::Clean() 
{
	tilemap->Clean();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "cleaned" << std::endl;
}