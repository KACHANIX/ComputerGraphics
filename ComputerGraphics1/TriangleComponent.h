#pragma once 
#include <d3d11.h>
#include <directxmath.h>
#include <vector>

#include "GameComponent.h" 

class Game;

class TriangleComponent : public GameComponent
{
	ID3D11InputLayout* layout_ = nullptr;

	ID3D11PixelShader* pixel_shader_ = nullptr;
	ID3DBlob* pixel_shader_byte_code_ = nullptr;

	ID3D11VertexShader* vertex_shader_ = nullptr;
	ID3DBlob* vertex_shader_byte_code_ = nullptr;

	ID3D11Buffer* vertices_ = nullptr;
	ID3D11Buffer* constant_buffer_ = nullptr;

	ID3D11RasterizerState* rast_state_ = nullptr;
	ID3D11Buffer* indeces_buffer = nullptr;
	DirectX::XMFLOAT4* points_;

public:

	TriangleComponent(Game* game);
	~TriangleComponent();

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float delta_time) override;
	virtual void Update(float delta_time) override;
	virtual void Reload() override {}
};