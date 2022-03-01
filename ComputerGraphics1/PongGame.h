#pragma once
#include "Game.h" 


/// <summary>
/// Just draws 2 triangles
/// </summary>
class PongGame :
    public Game
{
public:

    PongGame(LPCWSTR window_name) :Game(window_name) {}
    virtual void Initialize() override;
    virtual void Update(float delta_time) override;
    //virtual void Draw(float deltaTime) override;
    //virtual void PostDraw(float deltaTime) override;

};
