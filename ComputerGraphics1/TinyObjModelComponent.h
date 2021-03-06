#pragma once
#include "Camera.h" 
#include <d3d11.h> 

#include "TinyObjComponent.h" 
class TinyObjModelComponent : public TinyObjComponent
{

	ID3D11InputLayout* layout_ = nullptr;
	ID3D11PixelShader* pixel_shader_;
	ID3D11VertexShader* vertex_shader_;
	ID3DBlob* pixel_shader_byte_code_;
	ID3DBlob* vertex_shader_byte_code_;
	ID3D11Buffer* constant_buffer_;
	ID3D11SamplerState* sampler_;
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
public: 

	TinyObjModelComponent(Game* in_game, Camera* in_camera, char* in_file_name,
		float scale, float x_pos = 0, float z_pos = 0, bool in_is_main = false);
	~TinyObjModelComponent();

	virtual void Initialize() override;
	virtual void Update(float delta_time) override;
	virtual void Draw(float delta_time) override;
	virtual void DestroyResources() override;
	virtual void Reload() override {}
	void LoadTinyModel(const char* model_name, ID3D11Buffer*& v_buf,
		ID3D11Buffer*& n_buf, ID3D11Buffer*& t_buf,
		ID3D11Buffer*& str_buf, TinyMaterial*& materials,
		TinyShape*& shapes, int& elem_count, float& radius);
};
