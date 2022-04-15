#pragma once
#include "Game.h"

class LightSource
{
public:
	Game* game_;
	DirectX::SimpleMath::Vector3 look_at;
	DirectX::SimpleMath::Vector4 color;
	DirectX::SimpleMath::Matrix view_matrix;
	DirectX::SimpleMath::Matrix proj_matrix;
	DirectX::SimpleMath::Vector3 position;


	float ambient = 0.1f;
	float spec_pow = 50.0f;
	float spec_coef = 0.25f;

	LightSource();

	void GenerateViewMatrix();
	DirectX::SimpleMath::Matrix GetViewMatrix();
	DirectX::SimpleMath::Matrix GetProjMatrix();
	void SetLookAt(float x, float y, float z);
	void UpdateProjectionMatrix();

};