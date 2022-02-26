#pragma once

class Game;

class GameComponent
{
public:
	virtual ~GameComponent() = default;
	Game* game;

	GameComponent(Game* inGame) {
		game = inGame;
	}

	virtual void DestroyResources() = 0;
	virtual void Draw(float delta_time) = 0;
	virtual void Initialize() = 0;
	virtual void Reload() = 0;
	virtual void Update(float delta_time) = 0;
};