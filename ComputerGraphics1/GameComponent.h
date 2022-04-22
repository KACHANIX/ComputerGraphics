#pragma once
#include "SimpleMath.h"

class Camera;
class Game;

class GameComponent
{
public:
	virtual ~GameComponent() = default;
	Game* game;
	Camera* cam = nullptr;
	GameComponent(Game* in_game) {
		game = in_game;
	}
	GameComponent(Game* in_game, Camera* in_cam)
	{
		game = in_game;
		cam = in_cam;
	}

	DirectX::SimpleMath::Vector3 position;
	GameComponent* parent = nullptr;
	virtual void DestroyResources() = 0;
	virtual void Draw(float delta_time) = 0;
	virtual void DrawLight(float delta_time) = 0;

	virtual void Initialize() = 0;
	virtual void Reload() = 0;
	virtual void Update(float delta_time) = 0;
};