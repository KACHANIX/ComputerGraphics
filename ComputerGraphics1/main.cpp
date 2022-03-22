#include "TriangleComponent.h"
#include "Game.h"
#include "KatamariGame.h"
#include "PongGame.h"
#include "SolarGame.h"
#include "TriangleGame.h"


int main()
{ 
	//TriangleGame* game = new TriangleGame(L"My3DApp");
	//PongGame* game = new PongGame(L"ZDAROVA");
	//SolarGame* game = new SolarGame(L"ZDAROVA");
	KatamariGame* game = new KatamariGame(L"ZDAROVA");
	game->Run(1500	,1000);
	delete game;
	return 0;

}
