#pragma once
#include "SimpleMath.h"

class Game;
 
class Camera
{
	Game* game_;
public:
	DirectX::SimpleMath::Matrix view_matrix;
	DirectX::SimpleMath::Matrix proj_matrix;
	DirectX::SimpleMath::Matrix view_matrix_inverted;


	Camera(Game* in_game);
	~Camera();

	DirectX::SimpleMath::Matrix GetCameraMatrix() const;

	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetForwardVector() const;
	//DirectX::SimpleMath::Vector3 GetUpVector() const;

	void UpdateProjectionMatrix();
};
