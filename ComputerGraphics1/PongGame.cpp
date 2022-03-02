#include "PongGame.h" 

#include <iostream>

#include "PongBallComponent.h"  
#include "PongBallVelocity.h"
#include "PongPanelComponent.h" 


PongPanelComponent* pong_panel_l;
PongPanelComponent* pong_panel_r;
PongPanelComponent* pong_panel_t;
PongPanelComponent* pong_panel_b;
PongBallComponent* pong_ball;

void PongGame::Initialize()
{
	score_left = 0;
	score_right = 0;
	pong_panel_l = new PongPanelComponent(this, left);
	pong_panel_r = new PongPanelComponent(this, right);
	pong_panel_t = new PongPanelComponent(this, top);
	pong_panel_b = new PongPanelComponent(this, bottom);
	pong_ball = new PongBallComponent(this);
	components.push_back(pong_panel_l);
	components.push_back(pong_panel_r);
	components.push_back(pong_panel_t);
	components.push_back(pong_panel_b);
	components.push_back(pong_ball);
}

void PongGame::Update(float delta_time)
{
	if (pong_ball->GetConstData().x >= 0.97f)
	{
		score_left++;
		PrintScore();
		pong_ball->Reload(Slow, Zero);
	}
	else if (pong_ball->GetConstData().x <= -0.97f)
	{
		score_right++;
		PrintScore();
		pong_ball->Reload(Slow, Zero);
	}
	else
	{
		float ball_x = pong_ball->GetConstData().x;
		float ball_y = pong_ball->GetConstData().y;

		if ((ball_x >= 0.92f
			&& ball_y <= (pong_panel_r->GetConstData().y + 0.2f)
			&& ball_y >= (pong_panel_r->GetConstData().y - 0.2f)))
		{
			PanelHitBack(ball_y, pong_panel_r->GetConstData().y);
		}
		else if (ball_x <= -0.92f
			&& ball_y <= (pong_panel_l->GetConstData().y + 0.2f)
			&& ball_y >= (pong_panel_l->GetConstData().y - 0.2f))
		{
			PanelHitBack(ball_y, pong_panel_l->GetConstData().y);
		}

		if (abs(ball_y) >= 0.92f)
		{
			pong_ball->y_velocity *= -1.0f;
		}
	}
	Game::Update(delta_time);
}

void PongGame::PanelHitBack(float ball_y, float panel_const_y)
{

	pong_ball->x_velocity *= -1.0f;
	if (ball_y >= (panel_const_y + 0.12f))
	{
		pong_ball->y_velocity = Fast;

	}
	else if (ball_y >= (panel_const_y + 0.04f))
	{
		pong_ball->y_velocity = Slow;


	}
	else if (ball_y >= (panel_const_y - 0.04f))
	{
		pong_ball->y_velocity = Zero;

	}
	else if (ball_y >= (panel_const_y - 0.12f))
	{
		pong_ball->y_velocity = Slow * -1.0f;

	}
	else
	{
		pong_ball->y_velocity = Fast * -1.0f;

	}
}

void PongGame::PrintScore()
{
	std::cout << "Left: " << score_left << "  ----  " << score_right << " :Right" << std::endl;
}
