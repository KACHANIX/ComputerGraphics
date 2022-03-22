#include "KatamariGame.h" 
#include "Camera.h"
#include "FPSCameraController.h"
#include "PlaneComponent.h" 
#include "TinyObjModelComponent.h"
#include "TPCameraController.h"

void KatamariGame::Initialize()
{

	cam = new Camera(this);
	char* a = (char*)"../Objs/basketball.obj";
	GameComponent* t_obj = new TinyObjModelComponent(this, cam, a ,true);
	cam_controller = new TPCameraController(this, cam, t_obj);;

	PlaneComponent* plane = new PlaneComponent(this, cam, 50);
	components.push_back(plane);
	components.push_back(t_obj);
}
 
void KatamariGame::Update(float delta_time)
{
	cam_controller->Update(delta_time);
	Game::Update(delta_time);
}