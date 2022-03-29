#pragma once
#include "GameComponent.h"

struct TinyShape
{
	int StartIndex;
	int Count;
	int MaterialInd;
};

struct TinyMaterial
{
	const char* TexName;
	ID3D11Texture2D* DiffuseTexture;
	ID3D11ShaderResourceView* DiffSRV;
};
class TinyObjComponent :public GameComponent
{
public:
	bool is_possessed;
	float radius;
	float start_radius;
	DirectX::SimpleMath::Matrix transform = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Matrix parent_fixation;
	DirectX::SimpleMath::Matrix final_matrix;
	DirectX::SimpleMath::Vector3 parent_start_position;
	TinyObjComponent(Game* in_game) :GameComponent(in_game) {}
};