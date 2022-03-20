#pragma once
#include "Game.h" 
#include "InputDevice.h"

class FPSCameraController;
class Camera;

class KatamariGame : public Game
{
public:
	Camera* cam = nullptr;
	FPSCameraController* cam_controller = nullptr;

	KatamariGame(LPCWSTR window_name) :Game(window_name) {}
	virtual void Initialize() override;
	virtual void Update(float delta_time) override;

	void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};

