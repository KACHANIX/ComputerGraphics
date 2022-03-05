#pragma once
#include "Game.h"
#include "InputDevice.h"

class FPSCameraController;
class Camera;

class SolarGame : public Game
{
public:
	Camera* cam = nullptr;
	FPSCameraController* cam_controller = nullptr;

	SolarGame(LPCWSTR window_name):Game(window_name){}
	virtual void Initialize() override;
	virtual void Update(float delta_time) override;
	
	void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};

