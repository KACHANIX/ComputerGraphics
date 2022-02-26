
#include "TriangleGame.h"
#include "TriangleComponent.h"
 
TriangleComponent* triangle;

void TriangleGame::Initialize()
{
	triangle = new TriangleComponent(this);
	components.push_back(triangle);
}

void TriangleGame::Update(float deltaTime)
{
	Game::Update(deltaTime);
}