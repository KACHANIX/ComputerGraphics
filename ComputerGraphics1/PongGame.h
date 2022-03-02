#pragma once
#include "Game.h" 

 

/// <summary>
/// Just draws 2 triangles
/// </summary>
class PongGame :
    public Game
{
public:
    int score_left;
    int score_right;
    PongGame(LPCWSTR window_name) :Game(window_name) {}
    virtual void Initialize() override;
    virtual void Update(float delta_time) override;
    virtual void PrintScore();
    virtual void PanelHitBack(float ball_y, float panel_const_y);
    //virtual void Draw(float deltaTime) override;
    //virtual void PostDraw(float deltaTime) override;

};
