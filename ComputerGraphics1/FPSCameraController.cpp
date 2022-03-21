#include "FPSCameraController.h" 
#include "Game.h"

FPSCameraController::FPSCameraController(Game* in_game, Camera* in_camera)
{
	game_ = in_game;
	camera = in_camera;
	yaw = 0;
	pitch = 0;

	game_->input_device->MouseMove.AddRaw(this, &FPSCameraController::OnMouseMove); // TODO multiple controllers

	camera_position = DirectX::SimpleMath::Vector3(-2.0f, 0.0f, 0.0f);
}

void FPSCameraController::Update(float delta_time)
{
	//if (!game_->IsMainWindowActive())
	//{
	//	return;
	//}

	auto input = game_->input_device;
	auto rotation_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

	auto velocity_direction = DirectX::SimpleMath::Vector3::Zero;
	if (input->IsKeyDown(Keys::W))
	{
		velocity_direction += DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
	}
	if (input->IsKeyDown(Keys::A))
	{
		velocity_direction += DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
	}
	if (input->IsKeyDown(Keys::S))
	{
		velocity_direction += DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
	}
	if (input->IsKeyDown(Keys::D))
	{
		velocity_direction += DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
	}

	if (input->IsKeyDown(Keys::Space))
	{
		velocity_direction += DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	}
	if (input->IsKeyDown(Keys::C))
	{
		velocity_direction += DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f);
	}

	velocity_direction.Normalize();

	auto vel_dir = rotation_matrix.Forward() * velocity_direction.x
		+ DirectX::SimpleMath::Vector3::Up * velocity_direction.y
		+ rotation_matrix.Right() * velocity_direction.z;

	if (vel_dir.Length() != 0)
	{
		vel_dir.Normalize();
	}

	camera_position = camera_position + vel_dir * velocity_magnitude * delta_time;
	camera->view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(camera_position, camera_position + rotation_matrix.Forward(), rotation_matrix.Up());
	camera->UpdateProjectionMatrix();
}



void FPSCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	//if (!game_->IsMainWindowActive())
	//{
	//	return;
	//}
	if (game_->input_device->IsKeyDown(Keys::LeftShift))
	{
		return;
	}

	yaw -= args.Offset.x * 0.003 * mouse_sensitivity;
	pitch -= args.Offset.y * 0.003 * mouse_sensitivity;


	if (args.WheelDelta > 0)
	{
		velocity_magnitude += 10;
	}
	else if (args.WheelDelta < 0)
	{
		velocity_magnitude -= 10;
	}
}
