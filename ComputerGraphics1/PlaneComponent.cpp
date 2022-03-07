#include "PlaneComponent.h" 
#include "Game.h"
PlaneComponent::PlaneComponent(Game* in_game, Camera* in_camera, const int in_size) :GameComponent(in_game)
{
	camera_ = in_camera;
	position = DirectX::SimpleMath::Vector3::Zero;
	size_ = in_size;

	count_ = (size_ * 2 + 1) * 8;
	points_ = new DirectX::SimpleMath::Vector4[count_];


	DirectX::SimpleMath::Vector4 lines_color(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::SimpleMath::Vector4 main_line_color(0.0f, 0.0f, 1.0f, 1.0f);
	float step = 5.0f;
	float s = (step * size_);
	float x = -s;
	int i = 0;
	while (i < count_ / 2)
	{
		points_[i++] = DirectX::SimpleMath::Vector4(x, 0.0f, -s, 1.0f);
		points_[i++] = (x != 0.0f) ? lines_color : main_line_color;

		points_[i++] = DirectX::SimpleMath::Vector4(x, 0.0f, s, 1.0f);
		points_[i++] = (x != 0.0f) ? lines_color : main_line_color;
		x += step;
	}
	float z = -s;
	while (i < count_)
	{
		points_[i++] = DirectX::SimpleMath::Vector4(-s, 0.0f, z, 1.0f);
		points_[i++] = (z != 0.0f) ? lines_color : main_line_color;
		points_[i++] = DirectX::SimpleMath::Vector4(s, 0.0f, z, 1.0f);
		points_[i++] = (z != 0.0f) ? lines_color : main_line_color;
		z += step;
	}

}


PlaneComponent::~PlaneComponent()
{
	DestroyResources();
}

void PlaneComponent::Initialize()
{
	HRESULT res;

#pragma region Initialize shaders
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"../Shaders/SolarShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
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
	}
	res = game->device->CreateVertexShader(
		vertex_shader_byte_code_->GetBufferPointer(),
		vertex_shader_byte_code_->GetBufferSize(),
		nullptr, &vertex_shader_);

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"../Shaders/SolarShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
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
			MessageBox(game->display->hWnd, L"../Shaders/SolarShader.hlsl", L"Missing Shader File", MB_OK);
		}
	}
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
		&layout);
#pragma endregion Initialize layout

#pragma region Initialize buffers
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 32;
	bufDesc.ByteWidth = sizeof(DirectX::SimpleMath::Vector4) * count_;

	D3D11_SUBRESOURCE_DATA positionsData = {};
	positionsData.pSysMem = points_;
	positionsData.SysMemPitch = 0;
	positionsData.SysMemSlicePitch = 0;

	res = game->device->CreateBuffer(&bufDesc, &positionsData, &vertices_);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(DirectX::SimpleMath::Vector4) * count_;

	res = game->device->CreateBuffer(&constBufDesc, nullptr, &constant_buffer_);
#pragma endregion Initialize buffers

#pragma region Initialize rasterization
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->device->CreateRasterizerState(&rastDesc, &rast_state_);
	game->context->RSSetState(rast_state_);

#pragma endregion Initialize rasterization
}

void PlaneComponent::DestroyResources()
{
	if (layout != nullptr) layout->Release();
	if (pixel_shader_ != nullptr) pixel_shader_->Release();
	if (vertex_shader_ != nullptr) vertex_shader_->Release();
	if (pixel_shader_byte_code_ != nullptr) pixel_shader_byte_code_->Release();
	if (vertex_shader_byte_code_ != nullptr) vertex_shader_byte_code_->Release();
	if (vertices_ != nullptr) vertices_->Release();
	if (rast_state_ != nullptr) rast_state_->Release();
	if (constant_buffer_ != nullptr) constant_buffer_->Release();
}

void PlaneComponent::Draw(float delta_time)
{
	auto context = game->context;
	ID3D11RasterizerState* oldState;
	const UINT stride = 32;
	const UINT offset = 0;
	context->RSGetState(&oldState);
	context->RSSetState(rast_state_);
	context->IASetInputLayout(layout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	context->IASetVertexBuffers(0, 1, &vertices_, &stride, &offset);
	context->VSSetShader(vertex_shader_, nullptr, 0);
	context->PSSetShader(pixel_shader_, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &constant_buffer_);

	context->Draw(count_, 0);
	context->RSSetState(oldState);
	if (oldState != nullptr)
	{
		oldState->Release();
	}
}

void PlaneComponent::Update(float delta_time)
{
	auto wvp = DirectX::SimpleMath::Matrix::CreateTranslation(position) * camera_->view_matrix * camera_->proj_matrix;
	D3D11_MAPPED_SUBRESOURCE res = {};
	game->context->Map(constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	auto dataP = reinterpret_cast<float*>(res.pData);
	memcpy(dataP, &wvp, sizeof(DirectX::SimpleMath::Matrix));
	game->context->Unmap(constant_buffer_, 0);
}
