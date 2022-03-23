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
	GameComponent* t_obj = new TinyObjModelComponent(this, cam, (char*)"../Objs/basketball.obj",
		0.01f, 0, 0, true);
	GameComponent* t_obj_1 = new TinyObjModelComponent(this, cam, (char*)"../Objs/basketball.obj",
		0.005f, 15, 10);

	GameComponent* t_obj_2 = new TinyObjModelComponent(this, cam, (char*)"../Objs/scooter.obj",
		1.0f, -15, 10);
	cam_controller = new TPCameraController(this, cam, t_obj);;

	PlaneComponent* plane = new PlaneComponent(this, cam, 50);
	components.push_back(plane);
	components.push_back(t_obj);
	components.push_back(t_obj_1);
	components.push_back(t_obj_2);
}

void KatamariGame::Update(float delta_time)
{
	cam_controller->Update(delta_time);
	Game::Update(delta_time);
}