//#pragma warning //(??)

#include "Game.h"
#include "EnumID.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

Gamestate::ID Gamestate::StateOfGame::state;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Game* game = new Game();

	game->run();

	delete game;


	_CrtDumpMemoryLeaks();
	return 0;
}
