#pragma once
#include "InputDevice.h"
#include "SimpleMath.h"
#include "Camera.h"
#include "GameComponent.h"

class Game;

class TPCameraController
{
	Game* game_;
public:
	float yaw;
	float pitch;

	float velocity_magnitude = 100.0f;
	float mouse_sensitivity = 1.0f;

	Camera* camera;
	GameComponent* parent;
	//float camera_offset = 100.0f;
	DirectX::SimpleMath::Vector3 camera_position;

	TPCameraController(Game* in_game, ::Camera* in_camera, GameComponent*& in_parent);
	void Update(float delta_time);

protected:
	void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
};