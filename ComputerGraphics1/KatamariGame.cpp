#include "KatamariGame.h" 
#include "Camera.h"
#include "FPSCameraController.h"
#include "PlaneComponent.h" 
#include "TinyObjLightModelComponent.h"
#include "TinyObjModelComponent.h"
#include "TPCameraController.h"

void KatamariGame::Initialize()
{

	cam = new Camera(this);
	char* a = (char*)"../Objs/basketball.obj";
	GameComponent* t_obj = new TinyObjLightModelComponent(this, cam, (char*)"../Objs/basketball.obj",
		0.01f, 0, 0, true);
	GameComponent* t_obj_1 = new TinyObjModelComponent(this, cam, (char*)"../Objs/basketball.obj",
		0.005f, 5, 6); 

	GameComponent* t_obj_2 = new TinyObjModelComponent(this, cam, (char*)"../Objs/scooter.obj",
		1.3f, -5, 6);

	GameComponent* t_obj_3 = new TinyObjModelComponent(this, cam, (char*)"../Objs/basketball.obj",
		0.01f, 15, 0);

	GameComponent* t_obj_4 = new TinyObjModelComponent(this, cam, (char*)"../Objs/basketball.obj",
		0.015f, 10, 20);


	//GameComponent* t_obj_5 = new TinyObjModelComponent(this, cam, (char*)"../Objs/Wheel.obj",
	//	0.01f, -20, 20);
	//components.push_back(t_obj_5);
	//GameComponent* t_obj_6 = new TinyObjModelComponent(this, cam, (char*)"../Objs/Wheel.obj",
	//	0.03f, 20, 20);
	//components.push_back(t_obj_6);



	GameComponent* t_obj_7 = new TinyObjLightModelComponent(this, cam, (char*)"../Objs/scooter.obj",
		2.3f, -5, -10);
	cam_controller = new TPCameraController(this, cam, t_obj);;

	PlaneComponent* plane = new PlaneComponent(this, cam, 50);
	components.push_back(plane);
	components.push_back(t_obj);
	components.push_back(t_obj_1);
	components.push_back(t_obj_2);
	components.push_back(t_obj_3);
	components.push_back(t_obj_4);
	
	components.push_back(t_obj_7);
}

void KatamariGame::Update(float delta_time)
{
	cam_controller->Update(delta_time);
	Game::Update(delta_time);
}