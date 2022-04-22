#pragma once
#include "GameComponent.h"
#include "Camera.h" 
#include <d3d11.h> 

#include "LightSource.h"
#include "TinyObjComponent.h"

#pragma pack(push, 4)
struct ConstDataBuff
{
	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix wvp;
	DirectX::SimpleMath::Matrix lightvp;
	DirectX::SimpleMath::Vector4 viewer_position;
};
#pragma pack(pop)


#pragma pack(push, 4)
struct LightStruct
{
	DirectX::SimpleMath::Vector4 position;
	DirectX::SimpleMath::Vector4 color;
	DirectX::SimpleMath::Vector4 ka_spec_pow_ks_x;
};
#pragma pack(pop)

class TinyObjLightModelComponent : public TinyObjComponent
{

	ID3D11InputLayout* layout_ = nullptr;
	ID3D11PixelShader* pixel_shader_;
	ID3D11VertexShader* vertex_shader_;
	ID3DBlob* pixel_shader_byte_code_;
	ID3DBlob* vertex_shader_byte_code_;

	ID3D11PixelShader* pixel_shader_light_;
	ID3D11VertexShader* vertex_shader_light_;
	ID3DBlob* pixel_shader_byte_code_light_;
	ID3DBlob* vertex_shader_byte_code_light_;
	ID3D11Buffer* constant_buffer_;
	ID3D11Buffer* light_buffer_;
	ID3D11SamplerState* sampler_;
	ID3D11SamplerState* sampler2_;
	ID3D11RasterizerState* rast_state_;
	ID3D11BlendState* blend_state_;

	ID3D11Buffer* index_buffer_;
	ID3D11Buffer* v_buf_ = nullptr;
	ID3D11Buffer* n_buf_ = nullptr;
	ID3D11Buffer* t_buf_ = nullptr;
	ID3D11Buffer* str_buf_ = nullptr;

	ID3D11ShaderResourceView* v_SRV_ = nullptr;
	ID3D11ShaderResourceView* n_SRV_ = nullptr;
	ID3D11ShaderResourceView* t_SRV_ = nullptr;
	ID3D11ShaderResourceView* str_SRV_ = nullptr;

	TinyMaterial* materials_;
	TinyShape* shapes_;
	Camera* camera_;

	const char* model_name_;

	int elem_count_;
	float scale_;
	bool is_main_;

	LightSource* light_source_;

public:
	ConstDataBuff data = {};
	LightStruct light = {};
	

	TinyObjLightModelComponent(Game* in_game, Camera* in_camera, char* in_file_name,
		float scale, float x_pos , float z_pos ,LightSource* light_source_in, bool in_is_main = false);
	~TinyObjLightModelComponent();

	virtual void Initialize() override;
	virtual void Update(float delta_time) override;
	virtual void Draw(float delta_time) override;
	virtual void DrawLight(float delta_time) override;
	virtual void DestroyResources() override;
	virtual void Reload() override {}
	void LoadTinyModel(const char* model_name, ID3D11Buffer*& v_buf,
		ID3D11Buffer*& n_buf, ID3D11Buffer*& t_buf,
		ID3D11Buffer*& str_buf, TinyMaterial*& materials,
		TinyShape*& shapes, int& elem_count, float& radius);
};
