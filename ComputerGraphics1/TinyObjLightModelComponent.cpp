#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLightModelComponent.h"

#include <algorithm>

#include "Game.h"
#include "tiny_obj_loader.h"
#include <d3dcompiler.h>
#include <iostream>
#include "InputDevice.h"
#include "Keys.h"

 
TinyObjLightModelComponent::TinyObjLightModelComponent(Game* in_game, Camera* in_camera, char* in_file_name,
	float scale, float x_pos, float z_pos, bool in_is_main) :TinyObjComponent(in_game)
{
	scale_ = scale;
	transform *= DirectX::SimpleMath::Matrix::CreateScale(scale_);

	camera_ = in_camera;
	model_name_ = in_file_name;
	is_main_ = in_is_main;
	position = DirectX::SimpleMath::Vector3(x_pos, 0, z_pos);
}

void TinyObjLightModelComponent::LoadTinyModel(const char* model_name, ID3D11Buffer*& v_buf,
	ID3D11Buffer*& n_buf, ID3D11Buffer*& t_buf,
	ID3D11Buffer*& str_buf, TinyMaterial*& out_materials,
	TinyShape*& out_shapes, int& elem_count, float& radius)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	std::string directory;
	std::string fName(model_name);

	const size_t last_slash_idx = fName.rfind('/');
	if (std::string::npos != last_slash_idx)
	{
		directory = fName.substr(0, last_slash_idx) + "/";
	}
	//bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, model_name, directory.c_str());
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, model_name, directory.c_str());

	if (!warn.empty()) {
		std::cout << "OCHKO: WARN!" << std::endl;;
	}

	if (!err.empty())
	{
		std::cout << "OCHKO: ERR!" << std::endl;
	}

	if (!ret)
	{
		exit(1);
	}
	D3D11_BUFFER_DESC buf_desc = {};
	buf_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buf_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	buf_desc.CPUAccessFlags = 0;
	buf_desc.MiscFlags = 0;
	buf_desc.StructureByteStride = 0;
	buf_desc.ByteWidth = sizeof(float) * attrib.vertices.size();

	auto max = *std::max_element(std::begin(attrib.vertices), std::end(attrib.vertices));
	auto min = *std::min_element(std::begin(attrib.vertices), std::end(attrib.vertices));


	radius = max - min;
	D3D11_SUBRESOURCE_DATA res_data = {};
	res_data.pSysMem = &attrib.vertices[0];
	res_data.SysMemPitch = 0;
	res_data.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&buf_desc, &res_data, &v_buf);

	buf_desc.ByteWidth = sizeof(float) * attrib.normals.size();
	res_data.pSysMem = &attrib.normals[0];

	game->device->CreateBuffer(&buf_desc, &res_data, &n_buf);

	buf_desc.ByteWidth = sizeof(float) * attrib.texcoords.size();
	res_data.pSysMem = &attrib.texcoords[0];

	game->device->CreateBuffer(&buf_desc, &res_data, &t_buf);

	elem_count = shapes.size();
	out_shapes = new TinyShape[shapes.size()];
	std::vector<tinyobj::index_t> indexes;
	int ind = 0;
	for (auto shape : shapes)
	{
		out_shapes[ind].StartIndex = indexes.size();
		out_shapes[ind].Count = shape.mesh.indices.size();
		out_shapes[ind].MaterialInd = shape.mesh.material_ids[0];
		indexes.insert(std::end(indexes), std::begin(shape.mesh.indices), std::end(shape.mesh.indices));
		ind++;
	}


	buf_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	buf_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buf_desc.Usage = D3D11_USAGE_DEFAULT;
	buf_desc.StructureByteStride = sizeof(tinyobj::index_t);
	buf_desc.ByteWidth = sizeof(tinyobj::index_t) * indexes.size();
	res_data.pSysMem = &indexes[0];
	game->device->CreateBuffer(&buf_desc, &res_data, &str_buf);

	out_materials = new TinyMaterial[materials.size()];
	for (UINT i = 0; i < materials.size(); i++)
	{

		out_materials[i].TexName = materials[i].diffuse_texname.c_str();
		std::string diff_name = materials[i].diffuse_texname;
		if (diff_name.empty())
		{
			diff_name = "DefaultDiffuseMap.jpg";
		}
		auto texFile = directory + /*"/" +*/ diff_name;
		std::wstring stemp = std::wstring(texFile.begin(), texFile.end());
		game->texture_loader->LoadTextureFromFile(stemp.c_str(), out_materials[i].DiffuseTexture, out_materials[i].DiffSRV, false);
	}
}

void TinyObjLightModelComponent::Initialize()
{
	ID3DBlob* error_code = nullptr;

	auto res = D3DCompileFromFile(
		L"../Shaders/TinyLightShader.hlsl",
		nullptr,
		nullptr,
		"VSMain", "vs_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&vertex_shader_byte_code_, &error_code);

	if (FAILED(res))
	{
		if (error_code)
		{
			char* compileErrors = (char*)(error_code->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->display->hWnd, L"../Shaders/TinyLightShader.hlsl", L"Missing Shader File", MB_OK);
		}
		return;
	}

	game->device->CreateVertexShader(vertex_shader_byte_code_->GetBufferPointer(),
		vertex_shader_byte_code_->GetBufferSize(),
		nullptr,
		&vertex_shader_);


	res = D3DCompileFromFile(
		L"../Shaders/TinyLightShader.hlsl",
		nullptr,
		nullptr,
		"PSMain", "ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&pixel_shader_byte_code_, &error_code);

	if (FAILED(res))
	{
		if (error_code)
		{
			char* compileErrors = (char*)(error_code->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->display->hWnd, L"../Shaders/TinyLightShader.hlsl", L"Missing Shader File", MB_OK);
		}
		return;
	}


	game->device->CreatePixelShader(pixel_shader_byte_code_->GetBufferPointer(),
		pixel_shader_byte_code_->GetBufferSize(),
		nullptr,
		&pixel_shader_);
	v_buf_ = nullptr;
	n_buf_ = nullptr;
	t_buf_ = nullptr;
	str_buf_ = nullptr;
	materials_ = nullptr;
	shapes_ = nullptr;

	TinyObjLightModelComponent::LoadTinyModel(model_name_, v_buf_, n_buf_, t_buf_, str_buf_, materials_, shapes_, elem_count_, radius);
	radius *= scale_ * 0.5;
	start_radius = radius;
	std::cout << model_name_ << " radius:     " << radius << std::endl;


	D3D11_BUFFER_DESC desc_buf = {};
	v_buf_->GetDesc(&desc_buf);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	srv_desc.Buffer.FirstElement = 0;
	srv_desc.Buffer.NumElements = desc_buf.ByteWidth / sizeof(DirectX::SimpleMath::Vector3);

	game->device->CreateShaderResourceView(v_buf_, &srv_desc, &v_SRV_);

	n_buf_->GetDesc(&desc_buf);
	srv_desc.Buffer.NumElements = desc_buf.ByteWidth / sizeof(DirectX::SimpleMath::Vector3);
	game->device->CreateShaderResourceView(n_buf_, &srv_desc, &n_SRV_);

	t_buf_->GetDesc(&desc_buf);
	srv_desc.Buffer.NumElements = desc_buf.ByteWidth / sizeof(DirectX::SimpleMath::Vector2);
	srv_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
	game->device->CreateShaderResourceView(t_buf_, &srv_desc, &t_SRV_);

	str_buf_->GetDesc(&desc_buf);
	srv_desc.Buffer.NumElements = desc_buf.ByteWidth / desc_buf.StructureByteStride;
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	game->device->CreateShaderResourceView(str_buf_, &srv_desc, &str_SRV_);


	D3D11_BUFFER_DESC const_buf_desc = {};
	const_buf_desc.Usage = D3D11_USAGE_DEFAULT;
	const_buf_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	const_buf_desc.CPUAccessFlags = 0;
	const_buf_desc.MiscFlags = 0;
	const_buf_desc.StructureByteStride = 0;
	const_buf_desc.ByteWidth = sizeof(ConstDataBuff);

	game->device->CreateBuffer(&const_buf_desc, nullptr, &constant_buffer_);

	const_buf_desc.ByteWidth = sizeof(LightStruct);
	game->device->CreateBuffer(&const_buf_desc, nullptr, &light_buffer_);



	CD3D11_RASTERIZER_DESC rast_desc = {};
	rast_desc.CullMode = D3D11_CULL_FRONT; // TODO
	rast_desc.FillMode = D3D11_FILL_SOLID;
	//rast_desc.AntialiasedLineEnable = true;
	rast_desc.MultisampleEnable = true;
	game->device->CreateRasterizerState(&rast_desc, &rast_state_);

	D3D11_BLEND_DESC blend_desc = {};
	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	game->device->CreateBlendState(&blend_desc, &blend_state_);


	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = 1.0f;
	sampler_desc.BorderColor[1] = 0.0f;
	sampler_desc.BorderColor[2] = 0.0f;
	sampler_desc.BorderColor[3] = 1.0f;
	sampler_desc.MaxLOD = INT_MAX;

	game->device->CreateSamplerState(&sampler_desc, &sampler_);

	int total = 0;
	for (int i = 0; i < elem_count_; i++)
	{
		total += shapes_[i].Count;
	}

	int* indexes = new int[total];

	for (int i = 0; i < total; i++)
	{
		indexes[i] = i;
	}


	D3D11_BUFFER_DESC indDesc = {};
	indDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indDesc.CPUAccessFlags = 0;
	indDesc.MiscFlags = 0;
	indDesc.StructureByteStride = 0;
	indDesc.ByteWidth = sizeof(int) * total;

	D3D11_SUBRESOURCE_DATA indData = {};
	indData.pSysMem = indexes;
	indData.SysMemPitch = 0;
	indData.SysMemSlicePitch = 0;

	res = game->device->CreateBuffer(&indDesc, &indData, &index_buffer_);


	delete[] indexes;
}

void TinyObjLightModelComponent::Update(float delta_time)
{
	const float s = 0.05f;
	if (is_main_)
	{
		if (game->input_device->IsKeyDown(Keys::W))
		{
			transform *= transform.CreateRotationZ(-s);
			position += DirectX::SimpleMath::Vector3(s, 0.0f, 0.0f);
		}
		if (game->input_device->IsKeyDown(Keys::S))
		{
			transform *= transform.CreateRotationZ(s);
			position += DirectX::SimpleMath::Vector3(-s, 0.0f, 0.0f);
		}
		if (game->input_device->IsKeyDown(Keys::D))
		{
			transform *= transform.CreateRotationX(s);
			position += DirectX::SimpleMath::Vector3(0.0f, 0.0f, s);
		}
		if (game->input_device->IsKeyDown(Keys::A))
		{
			transform *= transform.CreateRotationX(-s);
			position += DirectX::SimpleMath::Vector3(0.0f, 0.0f, -s);
		}

		final_matrix = 1
			* transform
			* DirectX::SimpleMath::Matrix::CreateTranslation(position);
		for (int i = 2; i < game->components.size(); i++)
		{
			TinyObjComponent* child = (TinyObjComponent*)game->components[i];

			if (!child->is_possessed
				&& (child->position - position).Length() <= (child->radius + start_radius)
				&& child->radius < radius)
			{
				child->is_possessed = true;
				child->parent = this;
				radius += child->radius;

				child->parent_fixation = 1
					* transform.Invert()
					* child->final_matrix
					;
				child->parent_start_position = child->position - position;
			}
		}
	}
	else if (is_possessed)
	{
		TinyObjComponent* par = (TinyObjComponent*)parent;

		final_matrix = 1
			* DirectX::SimpleMath::Matrix::CreateTranslation(parent_start_position * (1 / scale_))
			* parent_fixation
			* par->final_matrix
			;
	}
	else
	{
		final_matrix = 1
			* transform
			* DirectX::SimpleMath::Matrix::CreateTranslation(position);
	}


	auto proj = final_matrix * camera_->GetCameraMatrix();

	data.world = final_matrix; // todo: check
	data.wvp = proj;

	const auto viewer_pos = camera_->GetPosition();
	data.viewer_position = DirectX::SimpleMath::Vector4(viewer_pos.x, viewer_pos.y, viewer_pos.z, 1.0f);

	game->context->UpdateSubresource(constant_buffer_, 0, nullptr, &data, 0, 0);

	DirectX::SimpleMath::Vector3 up_axis = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

	light.color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	light.direction = DirectX::SimpleMath::Vector4(light_direction.x, light_direction.y, light_direction.z, 0.0f); //todo check last arg 0 or 1
	//light.direction.Normalize();
	light.ka_spec_pow_ks_x = DirectX::SimpleMath::Vector4{ ambient, spec_pow, spec_coef, 0.0f };





	game->context->UpdateSubresource(light_buffer_, 0, nullptr, &light, 0, 0);
}


void TinyObjLightModelComponent::Draw(float delta_time)
{
	auto context = game->context;
	ID3D11RasterizerState* old_state;
	context->RSGetState(&old_state);
	context->RSSetState(rast_state_);

	float factors[] = { 1.0f,1.0f,1.0f,1.0f };
	context->OMSetBlendState(blend_state_, factors, 0xFFFFFFFF);

	context->IASetInputLayout(nullptr);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	context->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);

	context->VSSetShader(vertex_shader_, nullptr, 0);
	context->PSSetShader(pixel_shader_, nullptr, 0);

	ID3D11ShaderResourceView* srvs[] = { v_SRV_, n_SRV_, t_SRV_, str_SRV_ };
	context->VSSetShaderResources(0, 4, srvs);

	ID3D11Buffer* buffers[] ={constant_buffer_,light_buffer_};

	context->VSSetConstantBuffers(0, 1, &constant_buffer_);
	context->PSSetConstantBuffers(0, 2, buffers);
	context->PSSetSamplers(0, 1, &sampler_);

	for (int i = 0; i < elem_count_; i++)
	{
		auto shape = shapes_[i];
		auto material = materials_[shape.MaterialInd];

		context->PSSetShaderResources(0, 1, &material.DiffSRV);
		context->DrawIndexed(shape.Count, shape.StartIndex, 0);
	}

	context->RSSetState(old_state);
	if (old_state != nullptr)
	{
		old_state->Release();
	}

}

TinyObjLightModelComponent::~TinyObjLightModelComponent()
{
	TinyObjLightModelComponent::DestroyResources();
}

void TinyObjLightModelComponent::DestroyResources()
{
	delete[] materials_;
	delete[] shapes_;
	if (layout_ != nullptr) layout_->Release();
	if (pixel_shader_ != nullptr) pixel_shader_->Release();
	if (vertex_shader_ != nullptr) vertex_shader_->Release();
	if (pixel_shader_byte_code_ != nullptr) pixel_shader_byte_code_->Release();
	if (vertex_shader_byte_code_ != nullptr) vertex_shader_byte_code_->Release();
	if (constant_buffer_ != nullptr) constant_buffer_->Release();
	if (sampler_ != nullptr) sampler_->Release();
	if (rast_state_ != nullptr) rast_state_->Release();
	if (blend_state_ != nullptr) blend_state_->Release();
	if (index_buffer_ != nullptr) index_buffer_->Release();
	if (v_buf_ != nullptr) v_buf_->Release();
	if (n_buf_ != nullptr) n_buf_->Release();
	if (t_buf_ != nullptr) t_buf_->Release();
	if (str_buf_ != nullptr) str_buf_->Release();
	if (v_SRV_ != nullptr) v_SRV_->Release();
	if (n_SRV_ != nullptr) n_SRV_->Release();
	if (t_SRV_ != nullptr) t_SRV_->Release();
	if (str_SRV_ != nullptr) str_SRV_->Release();
	if (constant_buffer_ != nullptr) constant_buffer_->Release();
}

