#include "PlaneComponent.h" 
#include "Game.h"
PlaneComponent::PlaneComponent(Game* inGame, Camera* inCamera, int inSize) :GameComponent(inGame)
{
	camera = inCamera;
	Position = DirectX::SimpleMath::Vector3::Zero;
	size = inSize;
	count = (size * 2 + 1) * 8;
}


PlaneComponent::PlaneComponent(Game* inGame, Camera* inCamera) :GameComponent(inGame)
{
	camera = inCamera;
	Position = DirectX::SimpleMath::Vector3::Zero;
	count = (size * 2 + 1) * 8;
}

PlaneComponent::PlaneComponent(Game* inGame) : GameComponent(inGame)
{
	Position = DirectX::SimpleMath::Vector3::Zero;
	count = (size * 2 + 1) * 8;
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
		&vertexShaderByteCode,
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
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"../Shaders/SolarShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&pixelShaderByteCode,
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
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader); 
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
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout); 
#pragma endregion Initialize layout

#pragma region Initialize points value
	points = new DirectX::SimpleMath::Vector4[count * 16];

	int i = 0;
	std::cout << count << std::endl;
	std::cout << size << std::endl;
	float s = (float)size;
	float x = -s;
	while (i < (count / 2))
	{
		points[i] = DirectX::SimpleMath::Vector4(x, 0.0f, -s, 1.0f);
		i++;
		points[i] = (x != 0.0f) ? DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f) : DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		i++;

		points[i] = DirectX::SimpleMath::Vector4(x, 0.0f, s, 1.0f);
		i++;
		points[i] = (x != 0.0f) ? DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f) : DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		i++;

		x = x + 1.0f;
	}

	float z = -s;
	while (i < count)
	{
		points[i] = DirectX::SimpleMath::Vector4(-s, 0.0f, z, 1.0f);
		i++;
		points[i] = (z != 0.0f) ? DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f) : DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		i++;

		points[i] = DirectX::SimpleMath::Vector4(s, 0.0f, z, 1.0f);
		i++;
		points[i] = (z != 0.0f) ? DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f) : DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		i++;

		z = z + 1.0f;
	}
#pragma endregion Initialize points value

#pragma region Initialize bufferss
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 32;
	bufDesc.ByteWidth = sizeof(DirectX::SimpleMath::Vector4) * count;

	D3D11_SUBRESOURCE_DATA positionsData = {};
	positionsData.pSysMem = points;
	positionsData.SysMemPitch = 0;
	positionsData.SysMemSlicePitch = 0;

	res = game->device->CreateBuffer(&bufDesc, &positionsData, &vertices);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(DirectX::SimpleMath::Vector4) * count;

	res = game->device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);
#pragma endregion Initialize buffers

#pragma region Initialize rasterization state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID; // Only lines

	res = game->device->CreateRasterizerState(&rastDesc, &rastState); 
	res = game->context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation); 
#pragma endregion Initialize rasterization state
}

void PlaneComponent::DestroyResources()
{
	if (layout != nullptr) layout->Release();
	if (pixelShader != nullptr) pixelShader->Release();
	if (vertexShader != nullptr) vertexShader->Release();
	if (pixelShaderByteCode != nullptr) pixelShaderByteCode->Release();
	if (vertexShaderByteCode != nullptr) vertexShaderByteCode->Release();
	if (vertices != nullptr) vertices->Release();
	if (rastState != nullptr) rastState->Release();
	if (constantBuffer != nullptr) constantBuffer->Release();
	if (annotation != nullptr) annotation->Release();
}

void PlaneComponent::Draw(float deltaTime)
{
	auto context = game->context;
	ID3D11RasterizerState* oldState;
	const UINT stride = 32;
	const UINT offset = 0;
	context->RSGetState(&oldState);
	context->RSSetState(rastState);
	context->IASetInputLayout(layout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST); // Lines
	context->IASetVertexBuffers(0, 1, &vertices, &stride, &offset);
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &constantBuffer);

	annotation->BeginEvent(L"Wireframe draw event");
	context->Draw(count, 0);
	annotation->EndEvent();
	context->RSSetState(oldState);
	if (oldState != nullptr) oldState->Release();
}

void PlaneComponent::Update(float deltaTime)
{
	auto wvp = DirectX::SimpleMath::Matrix::CreateTranslation(Position) * camera->view_matrix * camera->proj_matrix;
	//game->Context->UpdataSubresource(constantBuffer, 0, nullptr, &wvp, 0, 0);
	D3D11_MAPPED_SUBRESOURCE res = {};
	game->context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	auto dataP = reinterpret_cast<float*>(res.pData);
	memcpy(dataP, &wvp, sizeof(DirectX::SimpleMath::Matrix));
	game->context->Unmap(constantBuffer, 0);
}
