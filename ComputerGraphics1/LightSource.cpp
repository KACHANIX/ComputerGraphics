

#include "LightSource.h"
#define M_PI 3.14159265358979323846

LightSource::LightSource(Game* in_game)
{
	position = DirectX::SimpleMath::Vector3(-30, 20, 0);
	color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	SetLookAt(0, 0, 0);
	game_ = in_game;

	GenerateViewMatrix();
	GenerateProjectionMatrix();

	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = game_->display->ClientWidth;
	depthTexDesc.Height = game_->display->ClientHeight;
	depthTexDesc.SampleDesc = { 1, 0 };
	game_->device->CreateTexture2D(&depthTexDesc, nullptr, &shadow_depth_buffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStenViewDesc = {};
	depthStenViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStenViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStenViewDesc.Flags = 0;
	game_->device->CreateDepthStencilView(shadow_depth_buffer, &depthStenViewDesc, &depth_view);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_Desc = {};
	srv_Desc.Format = DXGI_FORMAT_R32_FLOAT;
	srv_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_Desc.Texture2D.MostDetailedMip = 0;
	srv_Desc.Texture2D.MipLevels = 1;
	game_->device->CreateShaderResourceView(shadow_depth_buffer, &srv_Desc, &shadow_depth_resource_view);



	D3D11_DEPTH_STENCIL_DESC depthStenDesc = {};
	// Set up the description of the stencil state.
	depthStenDesc.DepthEnable = true;
	depthStenDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStenDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStenDesc.StencilEnable = true;
	depthStenDesc.StencilReadMask = 0xFF;
	depthStenDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStenDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStenDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStenDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStenDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStenDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStenDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStenDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStenDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	game_->device->CreateDepthStencilState(&depthStenDesc, &shadow_depth_state);
	game_->context->OMSetDepthStencilState(shadow_depth_state, 1);
}

void LightSource::GenerateViewMatrix()
{
	DirectX::SimpleMath::Vector3 up{ 0.0f,1.0f,0.0f };

	view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(position, look_at, up);
}

void LightSource::GenerateProjectionMatrix()
{
	proj_matrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView((float)M_PI / 2.0f, 1.0f, 0.1f, 10000.0f);
	//proj_matrix = DirectX::SimpleMath::Matrix::CreateOrthographic(500.0f, 500.0f, 0.1f, 10000.0f);
}

DirectX::SimpleMath::Matrix LightSource::GetViewMatrix()
{
	return view_matrix;
}

DirectX::SimpleMath::Matrix LightSource::GetProjMatrix()
{
	return proj_matrix;
}

void LightSource::SetLookAt(float x, float y, float z)
{
	look_at.x = x;
	look_at.y = y;
	look_at.z = z;
}
