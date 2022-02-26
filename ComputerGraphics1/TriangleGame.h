#pragma once
#include "Game.h"
#include "InputDevice.h" 

class TriangleGame :
    public Game
{ 
public: 

    TriangleGame(LPCWSTR window_name) :Game(window_name) {}
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    //virtual void Draw(float deltaTime) override;
    //virtual void PostDraw(float deltaTime) override;
    
};
