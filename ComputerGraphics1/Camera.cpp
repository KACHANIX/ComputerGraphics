#include "Camera.h"
#include "Game.h"
#define M_PI 3.14159265358979323846


Camera::Camera(Game* in_game)
{
	game_ = in_game;
	view_matrix = DirectX::SimpleMath::Matrix::Identity;
	UpdateProjectionMatrix();
}

Camera::~Camera()
{

}


DirectX::SimpleMath::Matrix Camera::GetCameraMatrix() const
{
	return view_matrix * proj_matrix;
}

DirectX::SimpleMath::Vector3 Camera::GetPosition() const
{
	DirectX::SimpleMath::Matrix inverted;
	view_matrix.Invert(inverted);
	return inverted.Translation();

}

DirectX::SimpleMath::Vector3 Camera::GetForwardVector() const
{
	DirectX::SimpleMath::Matrix inverted;
	view_matrix.Invert(inverted);
	return inverted.Up();
}

void Camera::UpdateProjectionMatrix()
{

	proj_matrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		(float)M_PI * 0.35f, 
		(float)(game_->display->ClientWidth) / (float)game_->display->ClientHeight,
		0.1f,
		10000.0f);
}

