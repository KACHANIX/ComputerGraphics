#include "Game.h"
#include "TriangleComponent.h"

#include <d3dcompiler.h>
#include <iostream>


TriangleComponent::TriangleComponent(Game* game) :GameComponent(game)
{
	points_ = new DirectX::SimpleMath::Vector4[10]
	{
		DirectX::SimpleMath::Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::SimpleMath::Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.5f, 1.0f),	DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::SimpleMath::Vector4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),

	};
}


TriangleComponent::~TriangleComponent()
{
	TriangleComponent::DestroyResources();
}

void TriangleComponent::DestroyResources()
{
	delete[] points_;

	if (layout_ != nullptr) layout_->Release();
	if (pixel_shader_ != nullptr) pixel_shader_->Release();
	if (vertex_shader_ != nullptr) vertex_shader_->Release();
	if (pixel_shader_byte_code_ != nullptr) pixel_shader_byte_code_->Release();
	if (vertex_shader_byte_code_ != nullptr) vertex_shader_byte_code_->Release();
	if (vertices_ != nullptr) vertices_->Release();
	if (constant_buffer_ != nullptr) constant_buffer_->Release();
}

void TriangleComponent::Initialize()
{
	HRESULT res;

#pragma region shaders 
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(
		L"../Shaders/MyShader.hlsl", 
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
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
			MessageBox(game->display->hWnd, L"../Shaders/MyShader.hlsl", L"Missing Shader File", MB_OK);
		}
		return;
	}

	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"../Shaders/MyShader.hlsl",
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
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
			MessageBox(game->display->hWnd, L"shader/Simple.hlsl", L"Missing Shader File", MB_OK);
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
	vertexBufDesc.ByteWidth = 10 * sizeof(DirectX::SimpleMath::Vector4);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points_;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	res = game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vertices_);

	int indeces[] = { 0,1,2, 2,3,4 };
	D3D11_BUFFER_DESC indDesc = {};
	indDesc.Usage = D3D11_USAGE_DEFAULT;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indDesc.CPUAccessFlags = 0;
	indDesc.MiscFlags = 0;
	indDesc.StructureByteStride = 0;
	indDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indData = {};
	indData.pSysMem = indeces;
	indData.SysMemPitch = 0;
	indData.SysMemSlicePitch = 0;

	res = game->device->CreateBuffer(&indDesc, &indData, &indeces_buffer);
#pragma endregion Initialize buffers

#pragma region Initialize rasterization
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->device->CreateRasterizerState(&rastDesc, &rast_state_);
	game->context->RSSetState(rast_state_);
#pragma endregion Initialize rasterization state
}

void TriangleComponent::Draw(float delta_time)
{

	auto context = game->context;
	UINT stride[] = { 32 };
	UINT offset[] = { 0 };

	context->RSSetState(rast_state_);

	context->IASetInputLayout(layout_);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(indeces_buffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, &vertices_, stride, offset);
	context->VSSetShader(vertex_shader_, nullptr, 0);
	context->PSSetShader(pixel_shader_, nullptr, 0);

	context->DrawIndexed(6, 0, 0);
}

void TriangleComponent::Update(float delta_time)
{
}