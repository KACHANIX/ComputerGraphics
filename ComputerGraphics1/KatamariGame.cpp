#include "KatamariGame.h"

#include "BoxComponent.h"
#include "Camera.h"
#include "FPSCameraController.h"
#include "PlaneComponent.h"
#include "SphereComponent.h"

PlaneComponent* plane;
void KatamariGame::Initialize()
{
	cam = new Camera(this);
	cam_controller = new FPSCameraController(this, cam);
	plane = new PlaneComponent(this, cam, 50);
	components.push_back(plane);
}

void KatamariGame::OnMouseMove(InputDevice::MouseMoveEventArgs& args)
{
}
void KatamariGame::Update(float delta_time)
{
	cam_controller->Update(delta_time);
	Game::Update(delta_time);
}