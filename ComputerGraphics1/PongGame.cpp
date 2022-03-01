#include "PongGame.h" 
#include "PongPanelComponent.h" 


PongPanelComponent* pong_panel_l;
PongPanelComponent* pong_panel_r;
PongPanelComponent* pong_panel_t;
PongPanelComponent* pong_panel_b;

void PongGame::Initialize()
{
	pong_panel_l = new PongPanelComponent(this, left);
	pong_panel_r = new PongPanelComponent(this, right);
	pong_panel_t = new PongPanelComponent(this, top);
	pong_panel_b = new PongPanelComponent(this, bottom);
	components.push_back(pong_panel_l);
	components.push_back(pong_panel_r);
	components.push_back(pong_panel_t);
	components.push_back(pong_panel_b);
}

void PongGame::Update(float delta_time)
{
	Game::Update(delta_time);
}