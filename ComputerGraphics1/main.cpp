#include "TriangleComponent.h"
#include "Game.h"
#include "PongGame.h"
#include "TriangleGame.h"


int main()
{ 
	//TriangleGame* game = new TriangleGame(L"My3DApp");

	PongGame* game = new PongGame(L"ZDAROVA");
	game->Run(800,800);
	delete game;
	return 0;

}
