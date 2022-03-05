#include "SolarGame.h"

#include "BoxComponent.h"
#include "Camera.h"
#include "FPSCameraController.h"
#include "PlaneComponent.h"

BoxComponent* box_1;
PlaneComponent* plane;
void SolarGame::Initialize()
{
	cam = new Camera(this);
	cam_controller = new FPSCameraController(this, cam);
	box_1 = new BoxComponent(this, cam);
	plane = new PlaneComponent(this, cam);
	components.push_back(box_1);
	components.push_back(plane);
}

void SolarGame::Update(float delta_time)
{
	cam_controller->Update(delta_time);
	Game::Update(delta_time);
}

