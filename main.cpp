#include "game.h"

Game *game = nullptr;

int main(int arc, char* argv[]) {

	game = new Game();

	game->Init("MonkEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while (game->GameRunning())
	{
		game->HandleEvents();
		game->Update();
		game->Render();

		
	}

	game->Clean();

	return 0;
}