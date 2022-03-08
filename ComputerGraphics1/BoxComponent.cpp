#include "BoxComponent.h"


#include "Game.h" 
#include <d3dcompiler.h>
#include <iostream> 

#include "Camera.h"
#include "InputDevice.h"


BoxComponent::BoxComponent(Game* in_game, Camera* in_cam, GameComponent* in_parent, float in_offset) :GameComponent(in_game, in_cam)
{
	offset = in_offset;
	if (in_parent != nullptr)
	{
		parent = in_parent;
	}
	position = DirectX::SimpleMath::Vector3(in_offset, 0, in_offset);
	points_quantity_ = 8;
	indices_quantity_ = 36;// 
	indices_ = new int[indices_quantity_]
	{
		0, 2, 1, 2, 3, 1,
			1, 3, 5, 3, 7, 5,
			2, 6, 3, 3, 6, 7,
			4, 5, 7, 4, 7, 6,
			0, 4, 2, 2, 4, 6,
			0, 1, 4, 1, 5, 4
	};
	points_ = new DirectX::SimpleMath::Vector4[points_quantity_ * 2]
	{
		DirectX::SimpleMath::Vector4(-2.5f, -2.5f, -2.5f, 1.0f),	DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(2.5f, -2.5f, -2.5f, 1.0f),	DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(-2.5f, -2.5f, 2.5f, 1.0f),	DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(2.5f, -2.5f, 2.5f, 1.0f),	DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),

		DirectX::SimpleMath::Vector4(-2.5f, 2.5f, -2.5f, 1.0f),	DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(2.5f, 2.5f, -2.5f, 1.0f),	DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(-2.5f, 2.5f, 2.5f, 1.0f),	DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(2.5f, 2.5f, 2.5f, 1.0f),		DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
	};
}


BoxComponent::~BoxComponent()
{
	BoxComponent::DestroyResources();
}

void BoxComponent::DestroyResources()
{
	delete[] points_;
	delete[] indices_;
	if (layout_ != nullptr) layout_->Release();
	if (pixel_shader_ != nullptr) pixel_shader_->Release();
	if (vertex_shader_ != nullptr) vertex_shader_->Release();
	if (pixel_shader_byte_code_ != nullptr) pixel_shader_byte_code_->Release();
	if (vertex_shader_byte_code_ != nullptr) vertex_shader_byte_code_->Release();
	if (vertices_buffer_ != nullptr) vertices_buffer_->Release();
	if (constant_buffer_ != nullptr) constant_buffer_->Release();
	if (old_state != nullptr) old_state->Release();
}

void BoxComponent::Initialize()
{
	HRESULT res;

#pragma region shaders 
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(
		L"../Shaders/SolarShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_DEBUG,
		0,
		&vertex_shader_byte_code_,
		&errorVertexCode);

	if (FAILED(res))
	{
		if (errorVertexCode)
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->display->hWnd, L"../Shaders/SolarShader.hlsl", L"Missing Shader File", MB_OK);
		}
		return;
	}

	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"../Shaders/SolarShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_DEBUG,
		0,
		&pixel_shader_byte_code_,
		&errorPixelCode);

	if (FAILED(res))
	{
		if (errorPixelCode)
		{
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->display->hWnd, L"shader/SolarShader.hlsl", L"Missing Shader File", MB_OK);
		}
	}

	res = game->device->CreateVertexShader(
		vertex_shader_byte_code_->GetBufferPointer(),
		vertex_shader_byte_code_->GetBufferSize(),
		nullptr, &vertex_shader_);

	res = game->device->CreatePixelShader(
		pixel_shader_byte_code_->GetBufferPointer(),
		pixel_shader_byte_code_->GetBufferSize(),
		nullptr, &pixel_shader_);

#pragma endregion Initialize shaders

#pragma region Initialize layout
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	res = game->device->CreateInputLayout(
		inputElements,
		2,
		vertex_shader_byte_code_->GetBufferPointer(),
		vertex_shader_byte_code_->GetBufferSize(),
		&layout_);

#pragma endregion Initialize layout

#pragma region Initialize buffers
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 32;
	vertexBufDesc.ByteWidth = points_quantity_ * 2 * sizeof(DirectX::SimpleMath::Vector4);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points_;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	res = game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vertices_buffer_);

	D3D11_BUFFER_DESC indDesc = {};
	indDesc.Usage = D3D11_USAGE_DEFAULT;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indDesc.CPUAccessFlags = 0;
	indDesc.MiscFlags = 0;
	indDesc.StructureByteStride = 0;
	indDesc.ByteWidth = sizeof(int) * indices_quantity_;

	D3D11_SUBRESOURCE_DATA indData = {};
	indData.pSysMem = indices_;
	indData.SysMemPitch = 0;
	indData.SysMemSlicePitch = 0;

	res = game->device->CreateBuffer(&indDesc, &indData, &indeces_buffer);

	D3D11_BUFFER_DESC const_buff_desc = {};
	const_buff_desc.Usage = D3D11_USAGE_DYNAMIC;
	const_buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	const_buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	const_buff_desc.MiscFlags = 0;
	const_buff_desc.StructureByteStride = 0;
	const_buff_desc.ByteWidth = sizeof(DirectX::SimpleMath::Matrix); //TODO CHECK MATRIX
	res = game->device->CreateBuffer(&const_buff_desc, nullptr, &constant_buffer_);

#pragma endregion Initialize buffers

#pragma region Initialize rasterization
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->device->CreateRasterizerState(&rastDesc, &rast_state_);
	game->context->RSSetState(rast_state_);
#pragma endregion Initialize rasterization state
}

void BoxComponent::Draw(float delta_time)
{

	auto context = game->context;
	UINT stride[] = { 32 };
	UINT offset[] = { 0 };

	context->RSGetState(&old_state);
	context->RSSetState(rast_state_);

	context->IASetInputLayout(layout_);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(indeces_buffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, &vertices_buffer_, stride, offset);
	context->VSSetShader(vertex_shader_, nullptr, 0);
	context->PSSetShader(pixel_shader_, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &constant_buffer_);
	context->DrawIndexed(indices_quantity_, 0, 0);

	context->RSSetState(old_state);
	if (old_state != nullptr)
	{
		old_state->Release();
	}
}

void BoxComponent::Update(float delta_time)
{
	// TODO TODO TODO TODO 1:48:00
	DirectX::SimpleMath::Matrix wvp;
	if (parent != nullptr)
	{
		if (parent->parent != nullptr)
		{
			wvp =
				DirectX::SimpleMath::Matrix::CreateTranslation(position)
				* DirectX::SimpleMath::Matrix::CreateRotationY(2 * game->total_time)
				* DirectX::SimpleMath::Matrix::CreateTranslation(parent->position)
				* DirectX::SimpleMath::Matrix::CreateRotationY(game->total_time)
				* cam->view_matrix * cam->proj_matrix;
		}
		else
		{
			wvp =
				DirectX::SimpleMath::Matrix::CreateTranslation(position)
				* DirectX::SimpleMath::Matrix::CreateTranslation(parent->position)
				* DirectX::SimpleMath::Matrix::CreateRotationY(game->total_time)
				* cam->view_matrix * cam->proj_matrix;
		}

	}
	else
	{
		wvp = DirectX::SimpleMath::Matrix::CreateTranslation(position)
			* DirectX::SimpleMath::Matrix::CreateRotationY(game->total_time)
			* cam->view_matrix * cam->proj_matrix;
	}



	D3D11_MAPPED_SUBRESOURCE res = {};
	game->context->Map(constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	auto data = reinterpret_cast<float*>(res.pData); // only if successful
	memcpy(data, &wvp, sizeof(DirectX::SimpleMath::Matrix));
	game->context->Unmap(constant_buffer_, 0);
}

void BoxComponent::Reload()
{}

BoxComponent::ConstData BoxComponent::GetConstData()
{
	return const_data_;
}
