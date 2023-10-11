#include "game.h"

Game *game = nullptr;


const int winWidth = 800;
const int winHeight = 640;

int main(int arc, char* argv[]) {

	game = new Game();

	
	game->Init("MonkEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, false);

	while (game->GameRunning())
	{
		game->HandleEvents();
		game->Update();
		game->Render();

		
	}

	game->Clean();

	return 0;
}