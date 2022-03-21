#include "KatamariGame.h" 
#include "Camera.h"
#include "FPSCameraController.h"
#include "PlaneComponent.h" 
#include "TinyObjModelComponent.h"

void KatamariGame::Initialize()
{

	cam = new Camera(this);
	cam_controller = new FPSCameraController(this, cam); 

	PlaneComponent* plane = new PlaneComponent(this, cam, 50);
	char* a = (char*)"../Objs/Scooter.obj";
	TinyObjModelComponent* t_obj = new TinyObjModelComponent(this, cam, a);
	components.push_back(plane);
	components.push_back(t_obj);
}
 
void KatamariGame::Update(float delta_time)
{
	cam_controller->Update(delta_time);
	Game::Update(delta_time);
}