#pragma once 
#include <d3d11_1.h>

#include "GameComponent.h"
#include "Camera.h"
#include "InputDevice.h"

class PlaneComponent :
	public GameComponent
{
	ID3D11InputLayout* layout = nullptr;

	ID3D11PixelShader* pixel_shader_ = nullptr;
	ID3DBlob* pixel_shader_byte_code_ = nullptr;

	ID3D11VertexShader* vertex_shader_ = nullptr;
	ID3DBlob* vertex_shader_byte_code_ = nullptr;

	ID3D11Buffer* vertices_ = nullptr;
	ID3D11Buffer* constant_buffer_ = nullptr;

	ID3D11RasterizerState* rast_state_ = nullptr;

	Camera* camera_ = nullptr;
	DirectX::SimpleMath::Vector4* points_ = nullptr;
	int size_;
	int count_;
public:
	DirectX::SimpleMath::Vector3 position;

	PlaneComponent(Game* in_game, Camera* in_camera, int size);

	~PlaneComponent();

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float delta_time) override;
	virtual void DrawLight(float delta_time) override{}
	virtual void Update(float delta_time) override;
	virtual void Reload() override {}
};
