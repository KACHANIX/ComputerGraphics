#pragma once
#include "Game.h" 
#include "InputDevice.h"

class TPCameraController;
class FPSCameraController;
class Camera;

class KatamariGame : public Game
{
public:
	Camera* cam = nullptr;
	TPCameraController* cam_controller = nullptr;

	KatamariGame(LPCWSTR window_name) :Game(window_name) {}
	virtual void Initialize() override;
	virtual void Update(float delta_time) override;

	void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};

