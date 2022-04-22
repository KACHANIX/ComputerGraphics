#pragma once 
#include <d3d11.h>
#include <directxmath.h> 

#include "GameComponent.h"  
#include "PongPanelSide.h"
#include "SimpleMath.h"


class Game;

class PongBallComponent : public GameComponent
{
	ID3D11InputLayout* layout_ = nullptr;

	ID3D11PixelShader* pixel_shader_ = nullptr;
	ID3DBlob* pixel_shader_byte_code_ = nullptr;

	ID3D11VertexShader* vertex_shader_ = nullptr;
	ID3DBlob* vertex_shader_byte_code_ = nullptr;

	ID3D11Buffer* vertices_buffer_ = nullptr;
	ID3D11Buffer* constant_buffer_ = nullptr;

	ID3D11RasterizerState* rast_state_ = nullptr;
	ID3D11Buffer* indeces_buffer = nullptr;
	DirectX::SimpleMath::Vector4* points_;
	PongPanelSide side_;
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

	float y_speed_ = 0;
public:
	float x_velocity = 0.01f;
	float y_velocity = 0.02f;
	PongBallComponent(Game* game);
	~PongBallComponent();

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float delta_time) override;
	virtual void DrawLight(float delta_time) override{}
	virtual void Update(float delta_time) override;
	virtual void Reload() override;
	virtual void Reload(float x, float y); 
	virtual ConstData GetConstData();
};