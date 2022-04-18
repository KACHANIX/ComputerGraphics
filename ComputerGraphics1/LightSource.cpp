

#include "LightSource.h"
#define M_PI 3.14159265358979323846

float width = 1000.0f;
float height = 1000.0f;
LightSource::LightSource(Game* in_game)
{
	position = DirectX::SimpleMath::Vector3(-100, 20, 0);
	color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	SetLookAt(0, 0, 0);
	game_ = in_game;
	UpdateProjectionMatrix();
	/*D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	srv_desc.Buffer.FirstElement = 0;
	srv_desc.Buffer.NumElements = desc_buf.ByteWidth / sizeof(DirectX::SimpleMath::Vector3);

	game->device->CreateShaderResourceView(v_buf_, &srv_desc, &v_SRV_);*/


	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = game_->display->ClientWidth;//  width;
	depthTexDesc.Height = game_->display->ClientHeight;// height; 
	depthTexDesc.SampleDesc = { 1, 0 };
	game_->device->CreateTexture2D(&depthTexDesc, nullptr, &shadow_depth_buffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStenViewDesc = {};
	depthStenViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStenViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStenViewDesc.Flags = 0;
	game_->device->CreateDepthStencilView(shadow_depth_buffer, &depthStenViewDesc, &depth_view);


	//D3D11_DEPTH_STENCIL_DESC depthStenDesc = {};
	//depthStenViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthStenDesc.DepthEnable = true;
	//depthStenDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStenDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//depthStenDesc.StencilEnable = true;
	//game_->device->CreateDepthStencilState(&depthStenDesc, &shadow_depth_state);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_Desc = {};
	srv_Desc.Format = DXGI_FORMAT_R32_FLOAT;
	srv_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//srv_Desc.Buffer.FirstElement = 0;
	//srv_Desc.Buffer.NumElements = desc_buf.ByteWidth / sizeof(DirectX::SimpleMath::Vector3); // TODO: WHAT


	game_->device->CreateShaderResourceView(shadow_depth_buffer, &srv_Desc, &shadow_depth_resource_view);
}

void LightSource::GenerateViewMatrix()
{
	DirectX::SimpleMath::Vector3 up{ 0.0f,1.0f,0.0f };

	view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(position, look_at, up);
}

void LightSource::UpdateProjectionMatrix()
{
	//TODO
	//proj_matrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView((float)M_PI / 2.0f, 1.0f, 0.1f, 10000.0f);
	proj_matrix = DirectX::SimpleMath::Matrix::CreateOrthographic(1000.0f, 1000.0f, 0.1f, 10000.0f);
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
