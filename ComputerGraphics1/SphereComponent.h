#pragma once
#include "GameComponent.h" 
#include <d3d11.h>

class Game;

class SphereComponent : public GameComponent
{

	ID3D11InputLayout* layout_ = nullptr;

	ID3D11PixelShader* pixel_shader_ = nullptr;
	ID3DBlob* pixel_shader_byte_code_ = nullptr;

	ID3D11VertexShader* vertex_shader_ = nullptr;
	ID3DBlob* vertex_shader_byte_code_ = nullptr;

	ID3D11Buffer* vertices_buffer_ = nullptr;
	ID3D11Buffer* constant_buffer_ = nullptr;

	ID3D11RasterizerState* old_state = nullptr;
	ID3D11RasterizerState* rast_state_ = nullptr;
	ID3D11Buffer* indeces_buffer = nullptr;
	DirectX::SimpleMath::Vector4* points_;
	int points_quantity_;
	int* indices_;
	int indices_quantity_;

#pragma pack(push, 4)
	struct ConstData
	{
		float x;
		float y;
		float dummy0;
		float dummy1;
	};
#pragma pack(pop)
	ConstData const_data_ = { 0,0 };

public:
	float offset;
	SphereComponent(Game* in_game, Camera* in_cam, GameComponent* in_parent = nullptr, float offset = 0);
	~SphereComponent();

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float delta_time) override;
	virtual void Update(float delta_time) override;
	virtual void Reload() override;
	virtual ConstData GetConstData();
};

