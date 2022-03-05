#pragma once
#include "InputDevice.h"
#include "SimpleMath.h"
#include "Camera.h"

class Game;

class FPSCameraController
{
	Game* game_;
public:
	float yaw;
	float pitch;

	float velocity_magnitude = 100.0f;
	float mouse_sensitivity = 1.0f;

	Camera* camera;
	DirectX::SimpleMath::Vector3 camera_position;

	FPSCameraController(Game* in_game, ::Camera* in_camera);
	void Update(float delta_time);

protected:
	void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
};