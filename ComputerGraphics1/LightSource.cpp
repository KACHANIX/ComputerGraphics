

#include "LightSource.h"
#define M_PI 3.14159265358979323846

LightSource::LightSource()
{
	position = DirectX::SimpleMath::Vector3(-100, 20, 0);
	color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	SetLookAt(0, 0, -1);
}

void LightSource::GenerateViewMatrix()
{
	DirectX::SimpleMath::Vector3 up{ 0.0f,1.0f,0.0f };

	view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(position, look_at, up);
}

void LightSource::UpdateProjectionMatrix()
{
	//proj_matrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView((float)M_PI / 2.0f, 1.0f, 0.1f, 10000.0f);
	proj_matrix = DirectX::SimpleMath::Matrix::CreateOrthographic(100.0f, 100.0f, 0.1f, 10000.0f);
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
