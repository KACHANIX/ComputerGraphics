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


	ID3D11Texture2D* shadow_depth_buffer = nullptr;
	ID3D11ShaderResourceView* shadow_depth_resource_view = nullptr; // r32float shaderResourceview
	ID3D11DepthStencilView* depth_view = nullptr;
	ID3D11DepthStencilState* shadow_depth_state; //d32float 
	float ambient = 0.1f;
	float spec_pow = 50.0f;
	float spec_coef = 0.25f;

	LightSource(Game* in_game);

	void GenerateViewMatrix();
	void GenerateProjectionMatrix();
	DirectX::SimpleMath::Matrix GetViewMatrix();
	DirectX::SimpleMath::Matrix GetProjMatrix();
	void SetLookAt(float x, float y, float z);

};