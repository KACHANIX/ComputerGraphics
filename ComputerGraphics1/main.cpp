#include "TriangleComponent.h"
#include "Game.h"
#include "TriangleGame.h"

int main()
{
	LPCWSTR applicationName = L"My3DApp";

	TriangleGame* tGame = new TriangleGame(L"My3DApp");
	tGame->Run(800,800);
	delete tGame;
	return 0;

}
