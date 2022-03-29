#include "TPCameraController.h"

#include "Game.h"

TPCameraController::TPCameraController(Game* in_game, Camera* in_camera, GameComponent*& in_parent)
{
	game_ = in_game;
	camera = in_camera;
	yaw = 0;
	pitch = 0;
	parent = in_parent;
	game_->input_device->MouseMove.AddRaw(this, &TPCameraController::OnMouseMove); // TODO multiple controllers

	//camera_position = DirectX::SimpleMath::Vector3(-2.0f, 0.0f, 0.0f);
}

void TPCameraController::Update(float delta_time)
{
	auto input = game_->input_device;
	auto rotation_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

	//auto velocity_direction = DirectX::SimpleMath::Vector3::Zero;
	//if (input->IsKeyDown(Keys::W))
	//{
	//	velocity_direction += DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
	//}
	//if (input->IsKeyDown(Keys::A))
	//{
	//	velocity_direction += DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
	//}
	//if (input->IsKeyDown(Keys::S))
	//{
	//	velocity_direction += DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
	//}
	//if (input->IsKeyDown(Keys::D))
	//{
	//	velocity_direction += DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
	//}

	//if (input->IsKeyDown(Keys::Space))
	//{
	//	velocity_direction += DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	//}
	//if (input->IsKeyDown(Keys::C))
	//{
	//	velocity_direction += DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f);
	//}

	//velocity_direction.Normalize();

	//auto vel_dir = rotation_matrix.Forward() * velocity_direction.x
	//	+ DirectX::SimpleMath::Vector3::Up * velocity_direction.y
	//	+ rotation_matrix.Right() * velocity_direction.z;

	//if (vel_dir.Length() != 0)
	//{
	//	vel_dir.Normalize();
	//}

	//camera_position = camera_position + vel_dir * velocity_magnitude * delta_time;
	//camera->view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(camera_position, camera_position + rotation_matrix.Forward(), rotation_matrix.Up());


	//DirectX::SimpleMath::Vector3 offset(-10.0f, 2.0f, 0.0f);
	DirectX::SimpleMath::Vector3 offset(-10.0f,  10.0f, 0.0f);

	camera_position = parent->position + offset;

	//yaw = 0;
	//pitch = 0;
	rotation_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

	camera->view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(camera_position,
		camera_position + rotation_matrix.Forward(),
		rotation_matrix.Up());


	camera->UpdateProjectionMatrix();
}



void TPCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
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
